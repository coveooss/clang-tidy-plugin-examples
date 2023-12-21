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
using ::clang::transformer::after;
using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::enclose;
using ::clang::transformer::makeRule;
using ::clang::transformer::node;
using ::CoveoClangTidyPluginsUtils::matchAliasType;

constexpr char className[] = "::CGL::AutoPtr";

using namespace ::clang::ast_matchers;

auto inline rewriteNewToStdMakeUnique()
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
        changeTo(cat("std::make_unique<", node(type), ">", ifBound(boundIfArgs, cat(enclose(after(node(type)), after(node(newExpr)))), cat("()")))),
        cat("Use std::make_unique<>() to create a new pointer."));
}

auto createReplaceAutoPtrByStdRule()
{
    return applyFirst({::CoveoClangTidyPluginsUtils::ReplaceMethodCallWithNoParamBuilder()
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
                       rewriteNewToStdMakeUnique(),
                       createReplacePtrAssignmentRule(className)});
}

// Boilerplate

class ReplaceAutoPtrByStdCheck : public TransformerClangTidyCheck
{
public:
    ReplaceAutoPtrByStdCheck(StringRef Name, ClangTidyContext* Context) : TransformerClangTidyCheck(createReplaceAutoPtrByStdRule(), Name, Context)
    {
    }
};

class ReplaceAutoPtrByStdCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<ReplaceAutoPtrByStdCheck>("coveo-replaceautoptrbystdcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::ReplaceAutoPtrByStdCheckModule> ReplaceAutoPtrByStdCheckInit("coveo-replaceautoptrbystdcheck-module",
                                                                                                   "Adds 'coveo-replaceautoptrbystdcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int ReplaceAutoPtrByStdCheckAnchorSource = 0;

}  // namespace clang::tidy
