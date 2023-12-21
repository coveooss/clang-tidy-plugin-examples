#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class AwesomePrefixCheck : public ClangTidyCheck
{
public:
    AwesomePrefixCheck(StringRef Name, ClangTidyContext* Context) : ClangTidyCheck(Name, Context)
    {
    }
    void registerMatchers(ast_matchers::MatchFinder* Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
};

void AwesomePrefixCheck::registerMatchers(MatchFinder* Finder)
{
    Finder->addMatcher(functionDecl().bind("add_awesome_prefix"), this);
}

void AwesomePrefixCheck::check(const MatchFinder::MatchResult& Result)
{
    const auto* MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("add_awesome_prefix");
    if (!MatchedDecl->getIdentifier() || MatchedDecl->getName().startswith("awesome_"))
        return;
    diag(MatchedDecl->getLocation(), "function %0 is insufficiently awesome") << MatchedDecl;
    diag(MatchedDecl->getLocation(), "insert 'awesome'", DiagnosticIDs::Note) << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "awesome_");
}

namespace {

class AwesomePrefixCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<AwesomePrefixCheck>("coveo-awesomeprefixcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::AwesomePrefixCheckModule> awesomePrefixCheckInit("coveo-awesomeprefixcheck-module",
                                                                                       "Adds 'coveo-awesomeprefixcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int awesomePrefixCheckAnchorSource = 0;

}  // namespace clang::tidy
