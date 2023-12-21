#pragma once

#include <cstdint>  // std::size_t
#include <initializer_list>
#include <iterator>  // std::make_move_iterator
#include <memory>    // std::make_shared
#include <string>
#include <utility>  // std::move
#include <vector>

#include "clang-tidy/utils/DeclRefExprUtils.h"  // allDeclRefExprs
#include "clang/AST/ComputeDependence.h"
#include "clang/AST/Decl.h"                       // FunctionDecl, VarDecl
#include "clang/ASTMatchers/ASTMatchersMacros.h"  // AST_MATCHER
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"              // BeforeThanCompare
#include "clang/Tooling/Transformer/RewriteRule.h"  // RewriteRuleWith
#include "clang/Tooling/Transformer/Stencil.h"      // StencilInterface
#include "llvm/Support/Casting.h"                   // dyn_cast

namespace CoveoClangTidyPluginsUtils {

AST_MATCHER_P(clang::CXXNewExpr, hasInitializer, clang::ast_matchers::internal::Matcher<clang::Expr>, InnerMatcher)
{
    const clang::Expr* Initializer = Node.getInitializer();
    return (Initializer != nullptr && InnerMatcher.matches(*Initializer, Finder, Builder));
}

/// Matches a node if the declaration reference associated with that node is
/// the first reference to a variable since its declaration.
///
/// Example matches x in if (x), but _not_ y in if (y)
/// \code
///   bool x, y;
///   y = false;
///   if (x) {}
///   if (y) {}
/// \endcode
AST_MATCHER(clang::DeclRefExpr, isFirstRefSinceDeclaration)
{
    using ::clang::tidy::utils::decl_ref_expr::allDeclRefExprs;
    auto variableDecl = llvm::dyn_cast<clang::VarDecl>(Node.getDecl());
    auto function     = variableDecl ? llvm::dyn_cast<clang::FunctionDecl>(variableDecl->getParentFunctionOrMethod()) : nullptr;
    auto functionBody = function ? function->getBody() : nullptr;

    clang::BeforeThanCompare<clang::SourceLocation> isBefore(Finder->getASTContext().getSourceManager());
    if (!variableDecl || !functionBody) {
        // The condition is redundant since (functionBody => variableDecl) , but it's more robust to changes this way
        return false;
    }

    for (const auto ref : allDeclRefExprs(*variableDecl, *functionBody, Finder->getASTContext())) {
        if (ref && isBefore(ref->getBeginLoc(), Node.getBeginLoc())) {
            return false;
        }
    }
    return true;
}

class U16ToCharStringLiteral : public clang::transformer::StencilInterface
{
public:
    explicit U16ToCharStringLiteral(clang::transformer::Stencil inner) : inner{std::move(inner)}
    {
    }

    std::string toString() const override
    {
        return "u16ToCharStringLiteral(" + inner->toString() + ")";
    }

    llvm::Error eval(const clang::ast_matchers::MatchFinder::MatchResult& match, std::string* result) const override
    {
        const std::size_t initialSize = result == nullptr ? 0 : result->size();
        auto              returnVal   = inner->eval(match, result);
        if (wasU16LiteralAddedAtPosition(result, initialSize)) {
            result->erase(initialSize, 1);  // erases "u"
        }
        return returnVal;
    }

private:
    bool wasU16LiteralAddedAtPosition(std::string* result, const std::size_t position) const
    {
        return result != nullptr && result->size() > position && result->substr(position, 2) == "u\"" && result->at(result->size() - 1) == '\"';
    }

    clang::transformer::Stencil inner{};
};

inline clang::transformer::Stencil u16ToCharStringLiteral(clang::transformer::Stencil inner)
{
    return std::make_shared<U16ToCharStringLiteral>(std::move(inner));
}

template<typename MetadataT> auto merge(std::initializer_list<std::vector<clang::transformer::RewriteRuleWith<MetadataT>>> rules)
{
    std::vector<clang::transformer::RewriteRuleWith<MetadataT>> result;
    for (auto&& rule : rules) {
        result.insert(result.end(), std::make_move_iterator(rule.begin()), std::make_move_iterator(rule.end()));
    }
    return result;
}

class ReplaceMethodCallWithNoParamBuilder
{
private:
    std::string oldMethod;
    std::string newMethod;
    std::string parentClass;

    auto buildStandard()
    {
        assert(!oldMethod.empty() && !newMethod.empty() && !parentClass.empty());
        using namespace ::clang::ast_matchers;
        const std::string object = "object";
        return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                 callExpr(callee(functionDecl(hasName(oldMethod), hasParent(recordDecl(hasName(parentClass))))),
                                          hasDescendant(memberExpr(has(expr().bind(object)))))),
                        changeTo(::clang::transformer::cat(::clang::transformer::access(object, ::clang::transformer::cat(newMethod, "()")))),
                        ::clang::transformer::cat("Replace ", parentClass, "::", oldMethod, "() method call with ", newMethod, "()"));
    }

    auto buildDependent()
    {
        assert(!oldMethod.empty() && !newMethod.empty() && !parentClass.empty());
        using namespace ::clang::ast_matchers;
        const std::string object = "object";
        return makeRule(
            traverse(clang::TK_IgnoreUnlessSpelledInSource,
                     callExpr(has(cxxDependentScopeMemberExpr(hasMemberName(oldMethod),
                                                              has(memberExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                                  templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(parentClass))))))))))),
                              hasDescendant(memberExpr(has(expr().bind(object)))))),
            changeTo(::clang::transformer::cat(::clang::transformer::node(object), ".", newMethod, "()")),
            ::clang::transformer::cat("Replace ", parentClass, "::", oldMethod, "() method call with ", newMethod, "()"));
    }

public:
    ReplaceMethodCallWithNoParamBuilder withOldMethodName(std::string rhsOldMethodName)
    {
        oldMethod = std::move(rhsOldMethodName);
        return *this;
    }
    ReplaceMethodCallWithNoParamBuilder withNewMethodName(std::string rhsNewMethodName)
    {
        newMethod = std::move(rhsNewMethodName);
        return *this;
    }
    ReplaceMethodCallWithNoParamBuilder withParentClass(std::string rhsParentClass)
    {
        parentClass = std::move(rhsParentClass);
        return *this;
    }

    auto build()
    {
        return ::clang::transformer::applyFirst({buildDependent(), buildStandard()});
    }
};

auto inline matchAliasType(const std::string& parentClass)
{
    using namespace ::clang::ast_matchers;
    return anyOf(hasType(elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(parentClass))))))),
                 hasType(type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName(parentClass))))))));
}

auto inline matchStdMove()
{
    using namespace ::clang::ast_matchers;
    return callExpr(callee(functionDecl(hasName("move"), isInStdNamespace())));
}

}  // namespace CoveoClangTidyPluginsUtils
