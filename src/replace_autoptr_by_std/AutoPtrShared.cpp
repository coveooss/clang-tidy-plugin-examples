#include <string>

#include "clang-tidy/utils/TransformerClangTidyCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Transformer/RangeSelector.h"  // name
#include "clang/Tooling/Transformer/RewriteRule.h"    // makeRule
#include "clang/Tooling/Transformer/Stencil.h"        // cat

#include <CoveoClangTidyPlugins/Utilities.h>

namespace AutoPtrClangTidyUtils {

using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::makeRule;
using ::clang::transformer::name;
using ::clang::transformer::node;
using ::clang::transformer::RewriteRuleWith;
using ::CoveoClangTidyPluginsUtils::matchAliasType;
using ::CoveoClangTidyPluginsUtils::matchStdMove;

using namespace ::clang::ast_matchers;

RewriteRuleWith<std::string> createRemoveIsNotNullBaseRule(const std::string& parentClass)
{
    const std::string object = "object";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             unaryOperator(hasOperatorName("!"),
                                           has(callExpr(anyOf(has(cxxDependentScopeMemberExpr(
                                                                  hasMemberName("IsNull"),
                                                                  has(memberExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(
                                                                      hasDeclaration(classTemplateDecl(hasName(parentClass))))))))))),
                                                              callee(functionDecl(hasName("IsNull"), hasParent(recordDecl(hasName(parentClass)))))),
                                                        hasDescendant(memberExpr(has(expr().bind(object)))))))),
                    changeTo(cat(node(object), ".get() != nullptr")),
                    cat("Remove !(", parentClass, "::IsNull()) method call and just check whether the pointer is not null"));
}

RewriteRuleWith<std::string> createRemoveIsNullBaseRule(const std::string& parentClass)
{
    const std::string object = "object";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 callExpr(unless(hasParent(unaryOperator(hasOperatorName("!")))),
                          anyOf(has(cxxDependentScopeMemberExpr(hasMemberName("IsNull"),
                                                                has(memberExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                                    templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(parentClass))))))))))),
                                callee(functionDecl(hasName("IsNull"), hasParent(recordDecl(hasName(parentClass)))))),
                          hasDescendant(memberExpr(has(expr().bind(object)))))),
        changeTo(cat(node(object), ".get() == nullptr")),
        cat("Remove ", parentClass, "::IsNull() method call and just check whether the pointer is null"));
}

RewriteRuleWith<std::string> createRemoveIsNullRule(const std::string& parentClass)
{
    return applyFirst({createRemoveIsNotNullBaseRule(parentClass), createRemoveIsNullBaseRule(parentClass)});
}

RewriteRuleWith<std::string> createUseStdMoveForReferenceConstructor(const std::string& parentClass)
{
    const std::string fullType     = "fulltype";
    const std::string otherPointer = "p";
    const std::string var          = "var";

    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 varDecl(has(elaboratedTypeLoc().bind(fullType)),
                         has(cxxConstructExpr(
                             matchAliasType(parentClass),
                             anyOf(has(expr(unless(hasDescendant(cxxNewExpr())), matchAliasType(parentClass), unless(matchStdMove())).bind(otherPointer)),
                                   has(conditionalOperator(matchAliasType(parentClass)).bind(otherPointer))))))
                     .bind(var)),

        changeTo(cat(node(fullType), " ", name(var), "(std::move(", node(otherPointer), "));")),
        cat("Use std::move() instead of passing other pointer by reference.")

    );
}

RewriteRuleWith<std::string> createReplaceExistingAssignmentWithMoveBaseRuleBinaryOp(const std::string& parentClass)
{
    const std::string lhs = "lhs";
    const std::string rhs = "rhs";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             binaryOperator(hasLHS(memberExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                                  templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(parentClass))))))))
                                                       .bind("lhs")),
                                            unless(hasDescendant(cxxNewExpr())),
                                            has(declRefExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType(
                                                                hasDeclaration(classTemplateDecl(anyOf(hasName(parentClass), hasName("std::unique_ptr")))))))))
                                                    .bind("rhs")))),
                    changeTo(cat(node(lhs), " = std::move(", node(rhs), ")")),
                    cat("Replace existing pointer reference assignment operator with std::move()."));
}

RewriteRuleWith<std::string> createReplaceExistingAssignmentWithMoveBaseRule(const std::string& parentClass)
{
    const std::string lhs = "lhs";
    const std::string rhs = "rhs";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 cxxOperatorCallExpr(matchAliasType(parentClass),
                                     isAssignmentOperator(),
                                     hasRHS(anyOf(expr(unless(hasDescendant(cxxNewExpr())), matchAliasType(parentClass), unless(matchStdMove())).bind(rhs),
                                                  conditionalOperator(matchAliasType(parentClass)).bind(rhs))),
                                     hasLHS(declRefExpr(matchAliasType(parentClass)).bind(lhs)))),
        changeTo(cat(name(lhs), " = std::move(", node(rhs), ")")),
        cat("Replace existing pointer reference assignment operator with std::move()."));
}

RewriteRuleWith<std::string> createReplaceAssignmentWithResetBaseRuleBinaryOp(const std::string& parentClass)
{
    const std::string lhs = "lhs";
    const std::string rhs = "rhs";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             binaryOperator(hasLHS(memberExpr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                                  templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(parentClass))))))))
                                                       .bind("lhs")),
                                            unless(hasDescendant(cxxNewExpr())),
                                            has(declRefExpr().bind("rhs")))),
                    changeTo(cat(node(lhs), ".reset(", node(rhs), ")")),
                    cat("Replace pointer assignment operator with reset() method."));
}

RewriteRuleWith<std::string> createReplaceAssignmentWithResetBaseRule(const std::string& parentClass)
{
    const std::string lhs = "lhs";
    const std::string rhs = "rhs";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             cxxOperatorCallExpr(matchAliasType(parentClass),
                                                 isAssignmentOperator(),
                                                 unless(hasDescendant(cxxNewExpr())),
                                                 hasRHS(expr(unless(anyOf(matchAliasType("std::unique_ptr"), matchAliasType(parentClass)))).bind(rhs)),
                                                 hasLHS(declRefExpr(matchAliasType(parentClass)).bind(lhs)))),
                    changeTo(cat(name(lhs), ".reset(", node(rhs), ")")),
                    cat("Replace pointer assignment operator with reset() method."));
}

// Orders the rules correctly since `createReplaceAssignmentWithResetBaseRule()` needs to be run after `createReplaceExistingAssignmentWithMoveBaseRule()`
// because it does similar matching except more general so it doesn't catch AutoPtr
RewriteRuleWith<std::string> createReplacePtrAssignmentRule(const std::string& parentClass)
{
    return applyFirst({createReplaceExistingAssignmentWithMoveBaseRule(parentClass),
                       createReplaceAssignmentWithResetBaseRule(parentClass),
                       createReplaceExistingAssignmentWithMoveBaseRuleBinaryOp(parentClass),
                       createReplaceAssignmentWithResetBaseRuleBinaryOp(parentClass)});
}

}  // namespace AutoPtrClangTidyUtils
