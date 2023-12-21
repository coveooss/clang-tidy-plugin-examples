#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/utils/TransformerClangTidyCheck.h"
#include "clang/Tooling/Transformer/RewriteRule.h"  // makeRule
#include "clang/Tooling/Transformer/Stencil.h"      // cat

namespace {

using ::clang::StringRef;
using ::clang::tidy::ClangTidyCheckFactories;
using ::clang::tidy::ClangTidyContext;
using ::clang::tidy::ClangTidyModule;
using ::clang::tidy::utils::TransformerClangTidyCheck;
using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::makeRule;

using namespace ::clang::ast_matchers;

constexpr auto coveoFilesystemHeader = "CoveoFilesystem/CoveoFilesystem.h";

auto createCGLGetModulePathReplacementRule()
{
    auto rule = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName("::CGL::CGLGetModulePath"))))),
                         changeTo(cat("CoveoFilesystem::GetExecutablePath()")),
                         cat("'::CGL::CGLGetModulePath' is deprecated in favor of 'CoveoFilesystem::GetExecutablePath'"));
    addInclude(rule, coveoFilesystemHeader);
    return rule;
}

auto createCGLGetModulePathWithDirectoryReplacementRule()
{
    auto rule = makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 cxxMemberCallExpr(on(callExpr(callee(functionDecl(hasName("::CGL::CGLGetModulePath"))))), callee(cxxMethodDecl(hasName("Directory"))))),
        changeTo(cat("CoveoFilesystem::GetExecutableDirectory()")),
        cat("'::CGL::CGLGetModulePath().Directory()' is deprecated in favor of 'CoveoFilesystem::GetExecutableDirectory()'"));
    addInclude(rule, coveoFilesystemHeader);
    return rule;
}

auto createCGLGetModuleStdPathReplacementRule()
{
    auto rule = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName("::CGL::CGLGetModuleStdPath"))))),
                         changeTo(cat("CoveoFilesystem::GetExecutablePath()")),
                         cat("'::CGL::CGLGetModuleStdPath' is deprecated in favor of 'CoveoFilesystem::GetExecutablePath'"));
    addInclude(rule, coveoFilesystemHeader);
    return rule;
}

auto createCGLGetModuleStdPathWithParentPathReplacementRule()
{
    auto rule = makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 cxxMemberCallExpr(on(callExpr(callee(functionDecl(hasName("::CGL::CGLGetModuleStdPath"))))), callee(cxxMethodDecl(hasName("parent_path"))))),
        changeTo(cat("CoveoFilesystem::GetExecutableDirectory()")),
        cat("'::CGL::CGLGetModuleStdPath().parent_path()' is deprecated in favor of 'CoveoFilesystem::GetExecutableDirectory()'"));
    addInclude(rule, coveoFilesystemHeader);
    return rule;
}

auto createExecutablePathRule()
{
    auto rule = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName("::CGLFile::Path::ExecutablePath"))))),
                         changeTo(cat("CoveoFilesystem::GetExecutableDirectory()")),
                         cat("'::CGLFile::Path::ExecutablePath' is deprecated in favor of 'CoveoFilesystem::GetExecutableDirectory'"));
    addInclude(rule, coveoFilesystemHeader);
    return rule;
}

auto createReplaceByGetExecutablePathRule()
{
    return applyFirst({
        createCGLGetModuleStdPathWithParentPathReplacementRule(),
        createCGLGetModulePathWithDirectoryReplacementRule(),

        createCGLGetModulePathReplacementRule(),
        createCGLGetModuleStdPathReplacementRule(),
        createExecutablePathRule(),
    });
}

// Boilerplate

class ReplaceByGetExecutablePathCheck : public TransformerClangTidyCheck
{
public:
    ReplaceByGetExecutablePathCheck(StringRef Name, ClangTidyContext* Context)
     : TransformerClangTidyCheck(createReplaceByGetExecutablePathRule(), Name, Context)
    {
    }
};

class ReplaceByGetExecutablePathCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<ReplaceByGetExecutablePathCheck>("coveo-replacebygetexecutablepathcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::ReplaceByGetExecutablePathCheckModule> replaceByGetExecutablePathCheckInit(
    "coveo-replacebygetexecutablepathcheck-module",
    "Adds 'coveo-replacebygetexecutablepathcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int replaceByGetExecutablePathCheckAnchorSource = 0;

}  // namespace clang::tidy
