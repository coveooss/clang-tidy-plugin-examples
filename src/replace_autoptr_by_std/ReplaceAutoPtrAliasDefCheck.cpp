#include <string>

#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/utils/TransformerClangTidyCheck.h"
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
using ::clang::transformer::before;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::enclose;
using ::clang::transformer::makeRule;
using ::clang::transformer::node;

constexpr char autoPtrClassName[]      = "::CGL::AutoPtr";
constexpr char arrayAutoPtrClassName[] = "::CGL::ArrayAutoPtr";

using namespace ::clang::ast_matchers;

auto matchVarDecl(const std::string& classNameToMatch, const std::string& innerTypeBind, const std::string& fullTypeBind, const std::string& fullDeclBind)
{
    return valueDecl(
               hasType(typeAliasTemplateDecl(hasName(classNameToMatch))),
               anyOf(has(elaboratedTypeLoc(hasNamedTypeLoc(templateSpecializationTypeLoc(hasTemplateArgumentLoc(0, hasTypeLoc(typeLoc().bind(innerTypeBind))))))
                             .bind(fullTypeBind)),
                     has(qualifiedTypeLoc(hasUnqualifiedLoc(elaboratedTypeLoc(hasNamedTypeLoc(templateSpecializationTypeLoc(
                                              templateSpecializationTypeLoc(hasTemplateArgumentLoc(0, hasTypeLoc(typeLoc().bind(innerTypeBind)))))))))
                             .bind(fullTypeBind)))

                   )
        .bind(fullDeclBind);
}

auto matchTypedefDecl(const std::string& classNameToMatch, const std::string& innerTypeBind, const std::string& fullTypeBind, const std::string& fullDeclBind)
{
    return typedefDecl(
               hasType(elaboratedType(hasDeclaration(typeAliasTemplateDecl(hasName(classNameToMatch))))),
               has(elaboratedTypeLoc(hasNamedTypeLoc(templateSpecializationTypeLoc(hasTemplateArgumentLoc(0, hasTypeLoc(typeLoc().bind(innerTypeBind))))))
                       .bind(fullTypeBind)))
        .bind(fullDeclBind);
}

auto replaceValueDeclType()
{
    const std::string fullDef   = "fulldef";
    const std::string fullType  = "fulltype";
    const std::string innerType = "innertype";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, matchVarDecl(autoPtrClassName, innerType, fullType, fullDef)),
                    changeTo(cat(enclose(before(node(fullDef)), before(node(fullType))),
                                 "std::unique_ptr<",
                                 node(innerType),
                                 ">",
                                 enclose(after(node(fullType)), after(node(fullDef))))),
                    cat("Replace a ", autoPtrClassName, " definition with std::unique_ptr"));
}

auto replaceValueDeclTypeArr()
{
    const std::string fullDef   = "fulldef";
    const std::string fullType  = "fulltype";
    const std::string innerType = "innertype";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, matchVarDecl(arrayAutoPtrClassName, innerType, fullType, fullDef)),
                    changeTo(cat(enclose(before(node(fullDef)), before(node(fullType))),
                                 "std::unique_ptr<",
                                 node(innerType),
                                 "[]>",
                                 enclose(after(node(fullType)), after(node(fullDef))))),
                    cat("Replace a ", arrayAutoPtrClassName, " definition with std::unique_ptr"));
}

auto replaceTypedefDeclType()
{
    const std::string fullDef   = "fulldef";
    const std::string fullType  = "fulltype";
    const std::string innerType = "innertype";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, matchTypedefDecl(autoPtrClassName, innerType, fullType, fullDef)),
                    changeTo(cat(enclose(before(node(fullDef)), before(node(fullType))),
                                 "std::unique_ptr<",
                                 node(innerType),
                                 ">",
                                 enclose(after(node(fullType)), after(node(fullDef))))),
                    cat("Replace a ", autoPtrClassName, " typedef with std::unique_ptr"));
}

auto replaceTypedefDeclTypeArr()
{
    const std::string fullDef   = "fulldef";
    const std::string fullType  = "fulltype";
    const std::string innerType = "innertype";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, matchTypedefDecl(arrayAutoPtrClassName, innerType, fullType, fullDef)),
                    changeTo(cat(enclose(before(node(fullDef)), before(node(fullType))),
                                 "std::unique_ptr<",
                                 node(innerType),
                                 "[]>",
                                 enclose(after(node(fullType)), after(node(fullDef))))),
                    cat("Replace a ", arrayAutoPtrClassName, " typedef with std::unique_ptr"));
}

auto createReplaceAutoPtrByStdRule()
{
    return applyFirst({replaceValueDeclType(), replaceValueDeclTypeArr(), replaceTypedefDeclType(), replaceTypedefDeclTypeArr()});
}

// Boilerplate

class ReplaceAutoPtrAliasDefCheck : public TransformerClangTidyCheck
{
public:
    ReplaceAutoPtrAliasDefCheck(StringRef Name, ClangTidyContext* Context) : TransformerClangTidyCheck(createReplaceAutoPtrByStdRule(), Name, Context)
    {
    }
};

class ReplaceAutoPtrAliasDefCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<ReplaceAutoPtrAliasDefCheck>("coveo-replaceautoptraliasdefcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::ReplaceAutoPtrAliasDefCheckModule> ReplaceAutoPtrAliasDefCheckInit("coveo-replaceautoptraliasdefcheck-module",
                                                                                                         "Adds 'coveo-replaceautoptraliasdefcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int ReplaceAutoPtrAliasDefCheckAnchorSource = 0;

}  // namespace clang::tidy
