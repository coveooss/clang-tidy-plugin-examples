#include <string>

#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/utils/TransformerClangTidyCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Transformer/RangeSelector.h"  // name
#include "clang/Tooling/Transformer/RewriteRule.h"    // makeRule
#include "clang/Tooling/Transformer/Stencil.h"        // cat

#include <CoveoClangTidyPlugins/Utilities.h>
#include "AutoPtrShared.h"

namespace {

using ::AutoPtrClangTidyUtils::createRemoveIsNullRule;
using ::AutoPtrClangTidyUtils::createReplacePtrAssignmentRule;
using ::AutoPtrClangTidyUtils::createUseStdMoveForReferenceConstructor;
using ::clang::StringRef;
using ::clang::tidy::ClangTidyCheckFactories;
using ::clang::tidy::ClangTidyContext;
using ::clang::tidy::ClangTidyModule;
using ::clang::tidy::utils::TransformerClangTidyCheck;
using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::makeRule;
using ::clang::transformer::node;
using ::CoveoClangTidyPluginsUtils::matchAliasType;

constexpr char className[] = "::CGL::ArrayAutoPtr";

using namespace ::clang::ast_matchers;

auto inline rewriteNewToStdMakeUniqueArray()
{
    const std::string newExpr     = "newExpr";
    const std::string type        = "type";
    const std::string fullType    = "fulltype";
    const std::string var         = "var";
    const std::string boundIfArgs = "boundIfArgs";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 cxxNewExpr(anyOf(hasAncestor(cxxConstructExpr(matchAliasType(className))),
                                  hasAncestor(cxxOperatorCallExpr(matchAliasType(className))),
                                  hasParent(binaryOperator(hasOperatorName("="),
                                                           hasDescendant(expr(hasType(elaboratedType(hasUnqualifiedDesugaredType(
                                                               templateSpecializationType(hasDeclaration(classTemplateDecl(hasName(className)))))))))))),
                            optionally(anyOf(has(parenListExpr().bind(boundIfArgs)),
                                             has(expr(unless(cxxConstructExpr())).bind(boundIfArgs)),
                                             has(cxxConstructExpr(expr(has(expr()))).bind(boundIfArgs)))),
                            hasTypeLoc(typeLoc().bind(type)))
                     .bind(newExpr)),
        changeTo(cat("std::make_unique<", node(type), "[]>(", node(boundIfArgs), ")")),
        cat("Use std::make_unique<>() to create a new array pointer."));
}

auto createReplaceStarRule()
{
    const std::string var = "var";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxOperatorCallExpr(hasOverloadedOperatorName("*"), has(expr(matchAliasType(className)).bind(var)))),
        changeTo(cat(node(var), "[0]")),
        cat("Replace ::CGL::ArrayAutoPtr star operator with the first element from the referenced array."));
}

auto createReplaceArrowRule()
{
    const std::string var = "var";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxOperatorCallExpr(hasOverloadedOperatorName("->"), has(expr(matchAliasType(className)).bind(var)))),
        changeTo(cat(node(var), "[0].")),
        cat("Replace ::CGL::ArrayAutoPtr arrow operator with a call on the first element from the referenced array."));
}

auto createReplaceAutoPtrByStdRule()
{
    return applyFirst({CoveoClangTidyPluginsUtils::ReplaceMethodCallWithNoParamBuilder()
                           .withOldMethodName("Release")
                           .withNewMethodName("release")
                           .withParentClass(className)
                           .build(),
                       ::CoveoClangTidyPluginsUtils::ReplaceMethodCallWithNoParamBuilder()
                           .withOldMethodName("Get")
                           .withNewMethodName("get")
                           .withParentClass(className)
                           .build(),
                       createRemoveIsNullRule(className),
                       createUseStdMoveForReferenceConstructor(className),
                       rewriteNewToStdMakeUniqueArray(),
                       createReplacePtrAssignmentRule(className),
                       createReplaceStarRule(),
                       createReplaceArrowRule()});
}

// Boilerplate

class ReplaceArrayAutoPtrByStdCheck : public TransformerClangTidyCheck
{
public:
    ReplaceArrayAutoPtrByStdCheck(StringRef Name, ClangTidyContext* Context) : TransformerClangTidyCheck(createReplaceAutoPtrByStdRule(), Name, Context)
    {
    }
};

class ReplaceArrayAutoPtrByStdCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<ReplaceArrayAutoPtrByStdCheck>("coveo-replacearrayautoptrbystdcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::ReplaceArrayAutoPtrByStdCheckModule> ReplaceArrayAutoPtrByStdCheckInit(
    "coveo-replacearrayautoptrbystdcheck-module",
    "Adds 'coveo-replacearrayautoptrbystdcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int ReplaceArrayAutoPtrByStdCheckAnchorSource = 0;

}  // namespace clang::tidy
