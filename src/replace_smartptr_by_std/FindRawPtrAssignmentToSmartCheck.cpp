#include <string>

#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/utils/TransformerClangTidyCheck.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Transformer/RangeSelector.h"  // name
#include "clang/Tooling/Transformer/RewriteRule.h"    // makeRule
#include "clang/Tooling/Transformer/Stencil.h"        // cat

#include <CoveoClangTidyPlugins/Utilities.h>

namespace {

using ::clang::StringRef;
using ::clang::tidy::ClangTidyCheckFactories;
using ::clang::tidy::ClangTidyContext;
using ::clang::tidy::ClangTidyModule;
using ::clang::tidy::utils::TransformerClangTidyCheck;
using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::makeRule;
using ::clang::transformer::node;
using ::clang::transformer::noopEdit;
using ::CoveoClangTidyPluginsUtils::matchAliasType;

constexpr auto className = "::CGL::SmartPtr";
constexpr auto msgCheck  = "Conversion from a raw pointer to a 'SmartPtr' is prone to double delete issues when 'SmartPtr' aliases `std::shared_ptr`.";

using namespace ::clang::ast_matchers;

auto createFindResetRule()
{
    const std::string callExprS = "cexpr";
    return makeRule(
        traverse(
            clang::TK_AsIs,
            callExpr(callee(functionDecl(hasName("reset"))),
                     anyOf(hasDescendant(expr(matchAliasType(className))),
                           hasDescendant(expr(hasType(pointerType(pointee(anyOf(
                               elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className)))))),
                               type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName(className))))))))))))),
                     argumentCountIs(1),
                     unless(cxxNullPtrLiteralExpr()),
                     unless(integerLiteral()),
                     unless(hasDescendant(cxxNullPtrLiteralExpr())))
                .bind(callExprS)),
        noopEdit(node(callExprS)),
        cat(msgCheck));
}

auto createFindRawPointerAssigmentOperatorCallExprRule()
{
    const std::string varExpr = "vexpr";
    return makeRule(
        traverse(
            clang::TK_AsIs,
            cxxOperatorCallExpr(
                isAssignmentOperator(),
                hasLHS(expr(matchAliasType(className))),
                hasRHS(expr(unless(cxxNullPtrLiteralExpr()), unless(integerLiteral()), unless(hasDescendant(cxxNullPtrLiteralExpr())), hasType(pointerType()))))
                .bind(varExpr)),
        noopEdit(node(varExpr)),
        cat(msgCheck));
}

auto createFindRawPointerAssigmentBinaryOperatorRule()
{
    const std::string varExpr = "vexpr";
    return makeRule(
        traverse(
            clang::TK_AsIs,
            binaryOperator(
                isAssignmentOperator(),
                hasLHS(expr(matchAliasType(className))),
                hasRHS(expr(unless(cxxNullPtrLiteralExpr()), unless(integerLiteral()), unless(hasDescendant(cxxNullPtrLiteralExpr())), hasType(pointerType()))))
                .bind(varExpr)),
        noopEdit(node(varExpr)),
        cat(msgCheck));
}

auto createFindRawPointerConstructorVarDeclRule()
{
    const std::string varExpr = "vexpr";
    return makeRule(
        traverse(
            clang::TK_AsIs,
            varDecl(
                matchAliasType(className),
                has(expr(unless(cxxNullPtrLiteralExpr()), unless(integerLiteral()), unless(hasDescendant(cxxNullPtrLiteralExpr())), hasType(pointerType()))))
                .bind(varExpr)),
        noopEdit(node(varExpr)),
        cat(msgCheck));
}

auto createFindRawPtrAssignmentRule()
{
    return applyFirst({createFindRawPointerAssigmentBinaryOperatorRule(), createFindRawPointerAssigmentOperatorCallExprRule()});
}

auto createFindRawPointerConstructorRule()
{
    const std::string constructExpr = "cexpr";
    return makeRule(traverse(clang::TK_AsIs,
                             cxxConstructExpr(matchAliasType(className),
                                              has(expr(unless(cxxNullPtrLiteralExpr()),
                                                       unless(integerLiteral()),
                                                       unless(hasDescendant(cxxNullPtrLiteralExpr())),
                                                       unless(cxxDefaultArgExpr()),
                                                       unless(cxxNewExpr()),
                                                       hasType(pointerType()))))
                                 .bind(constructExpr)),
                    noopEdit(node(constructExpr)),
                    cat(msgCheck));
}

auto createFindRawPtrAssignmentToSmartRule()
{
    return applyFirst(
        {createFindRawPointerConstructorRule(), createFindRawPointerConstructorVarDeclRule(), createFindRawPtrAssignmentRule(), createFindResetRule()});
}

// Boilerplate

class FindRawPtrAssignmentToSmartCheck : public TransformerClangTidyCheck
{
public:
    FindRawPtrAssignmentToSmartCheck(StringRef Name, ClangTidyContext* Context)
     : TransformerClangTidyCheck(createFindRawPtrAssignmentToSmartRule(), Name, Context)
    {
    }
};

class FindRawPtrAssignmentToSmartCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<FindRawPtrAssignmentToSmartCheck>("coveo-findrawptrassignmenttosmartcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::FindRawPtrAssignmentToSmartCheckModule> FindRawPtrAssignmentToSmartCheckInit(
    "coveo-findrawptrassignmenttosmartcheck-module",
    "Adds 'coveo-findrawptrassignmenttosmartcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int FindRawPtrAssignmentToSmartCheckAnchorSource = 0;

}  // namespace clang::tidy
