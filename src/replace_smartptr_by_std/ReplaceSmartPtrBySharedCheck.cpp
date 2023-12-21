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
using ::clang::transformer::after;
using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::enclose;
using ::clang::transformer::makeRule;
using ::clang::transformer::node;
using ::CoveoClangTidyPluginsUtils::matchAliasType;

constexpr char className[] = "::CGL::SmartPtr";

using namespace ::clang::ast_matchers;

auto inline rewriteImplicitConversionSPPointerReset()
{
    const std::string lhs = "lhs";
    const std::string rhs = "rhs";
    return makeRule(
        traverse(clang::TK_AsIs,
                 cxxOperatorCallExpr(
                     isAssignmentOperator(),
                     hasLHS(unaryOperator(
                         hasOperatorName("*"),
                         has(expr().bind(lhs)),
                         has(implicitCastExpr(hasType(pointerType(pointee(anyOf(
                             elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className)))))),
                             type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName(className)))))))))))))),
                     hasRHS(expr(hasType(pointerType()),
                                 unless(anyOf(cxxNewExpr(),
                                              cxxNullPtrLiteralExpr(),
                                              integerLiteral(),
                                              hasDescendant(cxxNewExpr()),
                                              hasDescendant(integerLiteral()),
                                              hasDescendant(cxxNullPtrLiteralExpr()))))
                                .bind(rhs)))),
        changeTo(cat(node(lhs), "->reset(", node(rhs), ")")),
        cat("Do not implicitly convert a SmartPtr to a raw pointer."));
}

auto inline rewriteImplicitConversionSPPointer()
{
    const std::string exp = "expr";
    return makeRule(
        traverse(
            clang::TK_AsIs,
            unaryOperator(unless(hasParent(cxxOperatorCallExpr(isAssignmentOperator()))),
                          hasOperatorName("*"),
                          has(expr().bind(exp)),
                          has(implicitCastExpr(hasType(pointerType(pointee(anyOf(
                              elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className)))))),
                              type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName(className)))))))))))))),
        changeTo(cat(node(exp), "->get()")),
        cat("Do not implicitly convert a SmartPtr to a raw pointer."));
}

auto inline rewriteImplicitConversion()
{
    const std::string expr = "expr";
    return makeRule(traverse(clang::TK_AsIs,
                             declRefExpr(matchAliasType(className),
                                         hasAncestor(cxxMemberCallExpr(hasDeclaration(decl(cxxConversionDecl())))),
                                         unless(hasParent(unaryOperator(hasOperatorName("*")))),
                                         unless(hasDescendant(unaryOperator(hasOperatorName("*")))))
                                 .bind(expr)),
                    changeTo(cat(node(expr), ".get()")),
                    cat("Do not implicitly convert a SmartPtr to a raw pointer."));
}

auto inline rewriteImplicitConversionDependent()
{
    const std::string expr = "expr";
    return makeRule(traverse(clang::TK_AsIs,
                             implicitCastExpr(matchAliasType(className),
                                              hasAncestor(cxxMemberCallExpr(hasDeclaration(decl(cxxConversionDecl())))),
                                              unless(hasAncestor(binaryOperator(isComparisonOperator()))),
                                              unless(hasParent(unaryOperator(hasOperatorName("*")))),
                                              unless(hasDescendant(unaryOperator(hasOperatorName("*")))))
                                 .bind(expr)),
                    changeTo(cat(node(expr), ".get()")),
                    cat("Do not implicitly convert a SmartPtr to a raw pointer."));
}

auto inline rewriteRawAssignmentToReset()
{
    const std::string rhs = "rhs";
    const std::string lhs = "lhs";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             expr(anyOf(binaryOperator(isAssignmentOperator(),
                                                       hasLHS(expr(matchAliasType(className)).bind(lhs)),
                                                       hasRHS(expr(hasType(pointerType()), unless(cxxNewExpr())).bind(rhs))),
                                        cxxOperatorCallExpr(isAssignmentOperator(),
                                                            hasLHS(expr(matchAliasType(className)).bind(lhs)),
                                                            hasRHS(expr(hasType(pointerType()), unless(cxxNewExpr())).bind(rhs)))))),
                    changeTo(cat(node(lhs), ".reset(", node(rhs), ")")),
                    cat("Use reset() to assign a raw pointer instead of equals."));
}

auto inline rewriteOperatorsToGet()
{
    const std::string var = "var";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             expr(matchAliasType(className),
                                  anyOf(hasParent(cxxOperatorCallExpr(isComparisonOperator(), hasRHS(declRefExpr(hasType(pointerType()))))),
                                        hasParent(cxxOperatorCallExpr(isComparisonOperator(), hasLHS(declRefExpr(hasType(pointerType())))))))
                                 .bind(var)),
                    changeTo(cat(node(var), ".get()")),
                    cat("When comparing a CGL::SmartPtr to a raw pointer compare the raw pointer with the value of the SmartPtr."));
}
// You could set the traverse mode to 'As_Is' to catch return statements and implicit constructions.
// However, it won't properly catch the nodes and rewrite them so about ~10% of the cases where
// it does those conversions will have to be fixed manually.
auto inline rewriteNewToCGLMakeSmart()
{
    const std::string newExpr     = "newExpr";
    const std::string typeB       = "type";
    const std::string fullType    = "fulltype";
    const std::string var         = "var";
    const std::string boundIfArgs = "boundIfArgs";
    return makeRule(
        traverse(
            clang::TK_IgnoreUnlessSpelledInSource,
            cxxNewExpr(

                anyOf(hasParent(binaryOperator(hasOperatorName("="),
                                               hasDescendant(expr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                   templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className))))))))))),
                      hasAncestor(varDecl(matchAliasType(className))),
                      hasAncestor(cxxConstructExpr(matchAliasType(className))),
                      hasAncestor(cxxOperatorCallExpr(matchAliasType(className))),
                      hasAncestor(returnStmt(hasReturnValue(matchAliasType(className)))),
                      hasAncestor(cxxOperatorCallExpr(hasLHS(unaryOperator(
                          hasOperatorName("*"),
                          has(implicitCastExpr(hasType(pointerType(pointee(anyOf(
                              elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className)))))),
                              type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName(className))))))))))))))))),
                optionally(anyOf(has(parenListExpr().bind(boundIfArgs)),
                                 has(expr(unless(cxxConstructExpr())).bind(boundIfArgs)),
                                 has(cxxConstructExpr(expr(has(expr()))).bind(boundIfArgs)))),
                hasTypeLoc(typeLoc().bind(typeB)))
                .bind(newExpr)),
        changeTo(cat("CGL::MakeSmartPtr<", node(typeB), ">", ifBound(boundIfArgs, cat(enclose(after(node(typeB)), after(node(newExpr)))), cat("()")))),
        cat("Use CGL::MakeSmartPtr<>() to create a new pointer."));
}

auto inline replaceStarEqNullPtrWithReset()
{
    const std::string lhs = "lhs";
    const std::string rhs = "rhs";
    return makeRule(
        traverse(clang::TK_AsIs,
                 cxxOperatorCallExpr(
                     isAssignmentOperator(),
                     hasLHS(unaryOperator(
                         hasOperatorName("*"),
                         has(expr().bind(lhs)),
                         has(implicitCastExpr(hasType(pointerType(pointee(anyOf(
                             elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className)))))),
                             type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName(className)))))))))))))),
                     hasRHS(anyOf(implicitCastExpr(has(integerLiteral())), implicitCastExpr(has(cxxNullPtrLiteralExpr())))))),
        changeTo(cat(node(lhs), "->reset()")),
        cat("Use reset() to null out a SmartPtr."));
}

auto inline replaceEqNullPtrWithReset()
{
    const std::string lhs = "lhs";
    return makeRule(
        traverse(clang::TK_AsIs,
                 expr(anyOf(binaryOperator(isAssignmentOperator(),
                                           hasLHS(expr(matchAliasType(className)).bind(lhs)),
                                           hasRHS(anyOf(implicitCastExpr(has(integerLiteral())), implicitCastExpr(has(cxxNullPtrLiteralExpr()))))),
                            cxxOperatorCallExpr(isAssignmentOperator(),
                                                hasLHS(expr(matchAliasType(className)).bind(lhs)),
                                                hasRHS(anyOf(implicitCastExpr(has(integerLiteral())), implicitCastExpr(has(cxxNullPtrLiteralExpr())))))))),
        changeTo(cat(node(lhs), ".reset()")),
        cat("Use reset() to null out a SmartPtr."));
}

auto inline rewriteSwapToLowerCase()
{
    const std::string object = "object";
    const std::string arg    = "arg";

    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 callExpr(anyOf(has(cxxDependentScopeMemberExpr(hasMemberName("Swap"),
                                                                has(memberExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                                    templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className))))))))))),
                                callee(functionDecl(hasName("Swap"), hasParent(recordDecl(hasName(className)))))),
                          hasDescendant(memberExpr(has(expr().bind(object)))),
                          has(declRefExpr().bind(arg)))),
        changeTo(cat(node(object), ".swap(", node(arg), ")")),
        cat("Replace ", className, "::Swap() method call with swap()"));
}

auto createReplaceSmartPtrBySharedRule()
{
    return applyFirst({rewriteImplicitConversionSPPointerReset(),
                       rewriteImplicitConversionSPPointer(),
                       rewriteImplicitConversion(),
                       rewriteSwapToLowerCase(),
                       rewriteOperatorsToGet(),
                       ::CoveoClangTidyPluginsUtils::ReplaceMethodCallWithNoParamBuilder()
                           .withOldMethodName("GetPtr")
                           .withNewMethodName("get")
                           .withParentClass(className)
                           .build(),
                       replaceStarEqNullPtrWithReset(),
                       rewriteNewToCGLMakeSmart(),
                       replaceEqNullPtrWithReset(),
                       rewriteRawAssignmentToReset(),
                       rewriteImplicitConversionDependent()});
}

// Boilerplate

class ReplaceSmartPtrBySharedCheck : public TransformerClangTidyCheck
{
public:
    ReplaceSmartPtrBySharedCheck(StringRef Name, ClangTidyContext* Context) : TransformerClangTidyCheck(createReplaceSmartPtrBySharedRule(), Name, Context)
    {
    }
};

class ReplaceSmartPtrBySharedCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<ReplaceSmartPtrBySharedCheck>("coveo-replacesmartptrbysharedcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::ReplaceSmartPtrBySharedCheckModule> ReplaceSmartPtrBySharedCheckInit("coveo-replacesmartptrbysharedcheck-module",
                                                                                                           "Adds 'coveo-replacesmartptrbysharedcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int ReplaceSmartPtrBySharedCheckAnchorSource = 0;

}  // namespace clang::tidy
