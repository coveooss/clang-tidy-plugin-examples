// Based on https://github.com/llvm-mirror/clang-tools-extra/blob/master/clang-tidy/add_new_check.py

#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/utils/TransformerClangTidyCheck.h"
#include "clang/Tooling/Transformer/RewriteRule.h" // makeRule
#include "clang/Tooling/Transformer/Stencil.h" // cat

namespace {

using ::clang::StringRef;
using ::clang::tidy::ClangTidyCheckFactories;
using ::clang::tidy::ClangTidyContext;
using ::clang::tidy::ClangTidyModule;
using ::clang::tidy::utils::TransformerClangTidyCheck;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::makeRule;
using ::clang::transformer::name;

using namespace ::clang::ast_matchers;

auto createAwesomePrefixRule() {
    constexpr auto f = "addAwesomePrefix";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, functionDecl(unless(matchesName("awesome_"))).bind(f)),
                         changeTo(name(f), cat("awesome_",  name(f))),
                         cat("function '", name(f), "' is insufficiently awesome"));
}

// Boilerplate

class AwesomePrefixCheck : public TransformerClangTidyCheck
{
public:
    AwesomePrefixCheck(StringRef Name, ClangTidyContext* Context)
     : TransformerClangTidyCheck(createAwesomePrefixRule(), Name, Context)
    {
    }
};

class AwesomePrefixCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<AwesomePrefixCheck>("awesomeprefixcheck");
    }
};

} // anonymous

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::AwesomePrefixCheckModule> awesomePrefixCheckInit("awesomeprefixcheck-module", "Adds 'awesomeprefixcheck'.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int awesomePrefixCheckAnchorSource = 0;

}  // namespace clang::tidy
