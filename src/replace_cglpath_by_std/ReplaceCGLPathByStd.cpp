#include <memory>
#include <string>
#include <utility>  // std::pair
#include <vector>

#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/utils/TransformerClangTidyCheck.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Transformer/RewriteRule.h"  // makeRule
#include "clang/Tooling/Transformer/Stencil.h"      // cat

#include <CoveoClangTidyPlugins/Utilities.h>

namespace {

using ::clang::StringRef;
using ::clang::tidy::ClangTidyCheckFactories;
using ::clang::tidy::ClangTidyContext;
using ::clang::tidy::ClangTidyModule;
using ::clang::tidy::utils::TransformerClangTidyCheck;
using ::clang::transformer::access;
using ::clang::transformer::applyFirst;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::makeRule;
using ::clang::transformer::maybeDeref;
using ::clang::transformer::node;
using ::clang::transformer::RewriteRuleWith;
using ::clang::transformer::selectBound;
using ::clang::transformer::Stencil;
using ::CoveoClangTidyPluginsUtils::isFirstRefSinceDeclaration;
using ::CoveoClangTidyPluginsUtils::merge;
using ::CoveoClangTidyPluginsUtils::u16ToCharStringLiteral;

using namespace ::clang::ast_matchers;

Stencil createDeprecatedMessage(const char* oldMethod, const char* newMethod)
{
    return cat("'", oldMethod, "' is deprecated in favor of '", newMethod, "'");
}

std::vector<RewriteRuleWith<std::string>> createRulesForMethodsWithNoArg()
{
    const std::vector<std::pair<const char*, const char*>> oldAndNewMethods = {
        {"::CGLFile::Path::FullPath", "u16string"},
        {"::CGLFile::Path::GenericFullPath", "generic_u16string"},
        {"::CGLFile::Path::FullPathA", "string"},
        {"::CGLFile::Path::GenericFullPathA", "generic_string"},
        {"::CGLFile::Path::Directory", "parent_path"},
        {"::CGLFile::Path::FileName", "filename"},
        {"::CGLFile::Path::FileNameNoExtension", "stem"},
        {"::CGLFile::Path::FileExtension", "extension"},
        {"::CGLFile::Path::Root", "root_path"},
        {"::CGLFile::Path::Empty", "empty"},
        {"::CGLFile::Path::Clear", "clear"},
        {"::CGLFile::Path::IsAbsolute", "is_absolute"},
        {"::CGLFile::Path::IsRelative", "is_relative"},
    };

    std::vector<RewriteRuleWith<std::string>> result;
    for (const auto& [oldMethod, newMethod] : oldAndNewMethods) {
        const std::string object = "object";
        result.emplace_back(
            makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxMemberCallExpr(on(expr().bind(object)), callee(cxxMethodDecl(hasName(oldMethod))))),
                     changeTo(cat(access(object, cat(newMethod, "()")))),
                     createDeprecatedMessage(oldMethod, newMethod)));
    }
    return result;
}

std::vector<RewriteRuleWith<std::string>> createRulesForMethodsWithNoArgToFunction()
{
    const std::vector<std::pair<const char*, const char*>> oldAndNewMethods = {
        {"::CGLFile::Path::Size", "file_size"},
        {"::CGLFile::Path::GetLastModificationDate", "CoveoFilesystem::GetLastModificationDate"},
        {"::CGLFile::Path::Exists", "exists"},
        {"::CGLFile::Path::HasDrive", "CoveoFilesystem::HasDrive"},
        {"::CGLFile::Path::IsDirectory", "is_directory"},
        {"::CGLFile::Path::IsSymbolicLink", "is_symlink"},
        {"::CGLFile::Path::IsFile", "is_regular_file"},
        {"::CGLFile::Path::IsReadOnly", "CoveoFilesystem::IsReadOnly"},
        {"::CGLFile::Path::IsWritable", "CoveoFilesystem::IsWritable"},
        {"::CGLFile::Path::GetCanonicalPath", "canonical"},
        {"::CGLFile::Path::CreateDir", "create_directories"},
        {"::CGLFile::Path::CreateFile", "CoveoFilesystem::CreateEmptyFile"},
        {"::CGLFile::Path::DeleteSymbolicLink", "remove"},
        {"::CGLFile::Path::Delete", "remove"},
        {"::CGLFile::Path::DeleteNoRetry", "remove"},
        {"::CGLFile::Path::RecursiveDelete", "remove_all"},
    };

    std::vector<RewriteRuleWith<std::string>> result;
    for (const auto& [oldMethod, newMethod] : oldAndNewMethods) {
        const std::string object = "object";
        result.emplace_back(
            makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))))),
                     changeTo(cat(newMethod, "(", maybeDeref(object), ")")),
                     createDeprecatedMessage(oldMethod, newMethod)));
    }
    return result;
}

std::vector<RewriteRuleWith<std::string>> createRulesForMethodsWithOneArgToFunctionWithThisAtTheEnd()
{
    const std::vector<std::pair<const char*, const char*>> oldAndNewMethods = {
        {"::CGLFile::Path::GetRelativePath", "relative"},
        {"::CGLFile::Path::CreateDirectorySymbolicLink", "create_directory_symlink"},
        {"::CGLFile::Path::CreateFileSymbolicLink", "create_symlink"},
    };

    std::vector<RewriteRuleWith<std::string>> result;
    for (const auto& [oldMethod, newMethod] : oldAndNewMethods) {
        const std::string object   = "object";
        const std::string argument = "argument";
        result.emplace_back(
            makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                              cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument)))),
                     changeTo(cat(newMethod, "(", node(argument), ", ", maybeDeref(object), ")")),
                     createDeprecatedMessage(oldMethod, newMethod)));
    }
    return result;
}

std::vector<RewriteRuleWith<std::string>> createRulesForMethodsOneArgToOperator()
{
    const std::vector<std::pair<const char*, const char*>> oldAndNewMethods = {
        {"::CGLFile::Path::Append", " /= "},
        {"::CGLFile::Path::Combine", " / "},
    };

    std::vector<RewriteRuleWith<std::string>> result;
    for (const auto& [oldMethod, newMethod] : oldAndNewMethods) {
        const std::string object   = "object";
        const std::string argument = "argument";
        result.emplace_back(
            makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                              cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument)))),
                     changeTo(cat(maybeDeref(object), newMethod, node(argument))),
                     createDeprecatedMessage(oldMethod, newMethod)));
    }
    return result;
}

class MethodsCopyRuleFactory
{
    const char* oldMethod = nullptr;
    const char* newMethod = nullptr;

    const std::string object        = "object";
    const std::string argument0     = "argument0";
    const std::string argument1     = "argument1";
    const std::string argumentFalse = "argumentFalse";
    const std::string argumentTrue  = "argumentTrue";

    auto createArgument0Matcher()
    {
        return hasArgument(0, expr().bind(argument0));
    }

    auto createArgument1Matcher()
    {
        return hasArgument(1,
                           anyOf(cxxBoolLiteral(equals(false)).bind(argumentFalse), cxxBoolLiteral(equals(true)).bind(argumentTrue), expr().bind(argument1)));
    }

    auto createArgument1Edit()
    {
        return selectBound(
            {{argumentTrue, cat(", std::filesystem::copy_options::overwrite_existing")},
             {argumentFalse, cat("")},
             {argument1, cat(", ", node(argument1), " ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none")}});
    }

    auto createMatcher()
    {
        return traverse(
            clang::TK_IgnoreUnlessSpelledInSource,
            cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher(), createArgument1Matcher()));
    }

    auto createMatcherDefault()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource,
                        cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher()));
    }

    auto createEdit()
    {
        return changeTo(cat(newMethod, "(", maybeDeref(object), ", ", node(argument0), createArgument1Edit(), ")"));
    }

    auto createEditDefault()
    {
        return changeTo(cat(newMethod, "(", maybeDeref(object), ", ", node(argument0), ", ", "std::filesystem::copy_options::overwrite_existing)"));
    }

public:
    MethodsCopyRuleFactory(const char* oldMethodRhs, const char* newMethodRhs) : oldMethod{oldMethodRhs}, newMethod{newMethodRhs}
    {
    }

    auto createRule()
    {
        return makeRule(createMatcher(), createEdit(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRuleDefault()
    {
        return makeRule(createMatcherDefault(), createEditDefault(), createDeprecatedMessage(oldMethod, newMethod));
    }
};

auto createRuleEquivalent()
{
    const auto        oldMethod = "::CGLFile::Path::Equivalent";
    const auto        newMethod = "equivalent";
    const std::string object    = "object";
    const std::string argument  = "argument";
    auto              rule      = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                  cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument)))),
                         changeTo(cat(newMethod, "(", maybeDeref(object), ", ", node(argument), ")")),
                         createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleChangeExtensionAppend()
{
    const std::string object   = "object";
    const std::string argument = "argument";
    auto              rule =
        makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                          cxxMemberCallExpr(on(expr().bind(object)),
                                            callee(functionDecl(hasName("::CGLFile::Path::ChangeExtension"))),
                                            hasArgument(0, expr().bind(argument)),
                                            hasArgument(1, declRefExpr(hasDeclaration(namedDecl(hasName("CGLFile::Path::APPEND"))))))),
                 changeTo(cat(access(object, cat("replace_extension(CGLFile::Path{", access(object, "extension().u16string()"), " + ", node(argument), "})")))),
                 cat("'::CGLFile::Path::ChangeExtension' is deprecated in favor of 'replace_extension'"));
    return rule;
}

auto createRuleChangeExtensionReplaceAll()
{
    const std::string object   = "object";
    const std::string argument = "argument";
    auto              rule     = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                  cxxMemberCallExpr(on(expr().bind(object)),
                                                    callee(functionDecl(hasName("::CGLFile::Path::ChangeExtension"))),
                                                    hasArgument(0, expr().bind(argument)),
                                                    hasArgument(1, declRefExpr(hasDeclaration(namedDecl(hasName("CGLFile::Path::REPLACE_ALL"))))))),
                         changeTo(cat("CoveoFilesystem::ReplaceAllExtensions(", maybeDeref(object), ", CGLFile::Path{", node(argument), "})")),
                         cat("'::CGLFile::Path::ChangeExtension' is deprecated in favor of 'CoveoFilesystem::ReplaceAllExtensions'"));
    return rule;
}

auto createRuleChangeExtensionDefaultArg()
{
    const std::string object   = "object";
    const std::string argument = "argument";
    auto              rule     = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                  cxxMemberCallExpr(on(expr().bind(object)),
                                                    callee(functionDecl(hasName("::CGLFile::Path::ChangeExtension"))),
                                                    hasArgument(0, expr().bind(argument)))),
                         changeTo(cat(access(object, cat("replace_extension(CGLFile::Path{", node(argument), "})")))),
                         cat("'::CGLFile::Path::ChangeExtension' is deprecated in favor of 'replace_extension'"));
    return rule;
}

auto createRuleContains()
{
    const auto        oldMethod = "::CGLFile::Path::Contains";
    const auto        newMethod = "CoveoFilesystem::BasePathContains";
    const std::string object    = "object";
    const std::string argument  = "argument";
    auto              rule      = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                  cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument)))),
                         changeTo(cat(newMethod, "(", access(object, "u16string()"), ", ", access(argument, "u16string()"), ")")),
                         createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleReadSymbolicLink()
{
    const auto        oldMethod = "::CGLFile::Path::ReadSymbolicLink";
    const auto        newMethod = "read_symlink";
    const std::string object    = "object";
    const std::string argument  = "argument";
    auto              rule      = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                  cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument)))),
                         changeTo(cat(node(argument), " = ", newMethod, "(", maybeDeref(object), ")")),
                         createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

class GetFileListSizeRuleFactory
{
    const char* oldMethod = "::CGLFile::Path::GetFileListSize";
    const char* newMethod = "CoveoFilesystem::GetTotalFilesSize";

    const std::string object                 = "object";
    const std::string argument0              = "argument0";
    const std::string argument0StringLiteral = "argument0StringLiteral";
    const std::string argument1              = "argument1";
    const std::string argument1False         = "argument1False";
    const std::string argument1True          = "argument1True";

    auto createArgument0Matcher()
    {
        return hasArgument(0, anyOf(stringLiteral().bind(argument0StringLiteral), expr().bind(argument0)));
    }

    auto createArgument0Edit()
    {
        return selectBound({{argument0StringLiteral, u16ToCharStringLiteral(cat(node(argument0StringLiteral)))},
                            {argument0, cat("CGLCharset::Conv::ConvertU16ToU8StringCopy(", node(argument0), ")")}});
    }

    auto createArgument1Matcher()
    {
        return hasArgument(1,
                           anyOf(cxxBoolLiteral(equals(false)).bind(argument1False), cxxBoolLiteral(equals(true)).bind(argument1True), expr().bind(argument1)));
    }

    auto createArgument1Edit()
    {
        return selectBound(
            {{argument1True, cat(", CoveoFilesystem::FileListingOptions::Recursive")},
             {argument1False, cat("")},
             {argument1, cat(", ", node(argument1), " ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None")}});
    }

    auto createEditGeneral()
    {
        return changeTo(cat(newMethod, "(", maybeDeref(object), ", ", createArgument0Edit(), createArgument1Edit(), ")"));
    }

    auto createEdit1Default()
    {
        return changeTo(cat(newMethod, "(", maybeDeref(object), ", ", createArgument0Edit(), ")"));
    }

    auto createEdit2Defaults()
    {
        return changeTo(cat(newMethod, "(", maybeDeref(object), ")"));
    }

    auto createMatcherGeneral()
    {
        return traverse(
            clang::TK_IgnoreUnlessSpelledInSource,
            cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher(), createArgument1Matcher()));
    }

    auto createMatcher1Default()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource,
                        cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher()));
    }

    auto createMatcher2Defaults()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod)))));
    }

public:
    GetFileListSizeRuleFactory() = default;

    auto createRuleGeneral()
    {
        return makeRule(createMatcherGeneral(), createEditGeneral(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRule1Default()
    {
        return makeRule(createMatcher1Default(), createEdit1Default(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRule2Defaults()
    {
        return makeRule(createMatcher2Defaults(), createEdit2Defaults(), createDeprecatedMessage(oldMethod, newMethod));
    }
};

class GetFileListRuleFactory
{
    const char* oldMethod = "::CGLFile::Path::GetFileList";
    const char* newMethod = "CoveoFilesystem::GetFileList";

    const std::string object                 = "object";
    const std::string argument0              = "argument0";
    const std::string argument0Empty         = "argument0Empty";
    const std::string argument1              = "argument1";
    const std::string argument1StringLiteral = "argument1StringLiteral";
    const std::string argument2              = "argument2";
    const std::string argument2False         = "argument2False";
    const std::string argument2True          = "argument2True";
    const std::string argument3              = "argument3";

    auto createArgument0Matcher()
    {
        return hasArgument(0,
                           anyOf(declRefExpr(allOf(isFirstRefSinceDeclaration(),
                                                   hasDeclaration(varDecl(allOf(hasLocalStorage(), hasInitializer(cxxConstructExpr(argumentCountIs(0))))))))
                                     .bind(argument0Empty),
                                 expr().bind(argument0)));
    }

    auto createArgument0Edit(Stencil functionCall)
    {
        return cat(selectBound({
            {argument0Empty, cat(node(argument0Empty), " = ", functionCall)},
            {argument0,
             cat("{\nstd::vector<Path> tmp = ",
                 functionCall,
                 ";\n",
                 access(argument0,
                        cat("insert(", access(argument0, "end()"), ", std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));\n}")))},
        }));
    }

    auto createArgument1Matcher()
    {
        return hasArgument(1, anyOf(stringLiteral().bind(argument1StringLiteral), expr().bind(argument1)));
    }

    auto createArgument1Edit()
    {
        return selectBound({{argument1StringLiteral, u16ToCharStringLiteral(cat(node(argument1StringLiteral)))},
                            {argument1, cat("CGLCharset::Conv::ConvertU16ToU8StringCopy(", node(argument1), ")")}});
    }

    auto createArgument2Matcher()
    {
        return hasArgument(2,
                           anyOf(cxxBoolLiteral(equals(false)).bind(argument2False), cxxBoolLiteral(equals(true)).bind(argument2True), expr().bind(argument2)));
    }

    auto createArgument2Edit()
    {
        return selectBound(
            {{argument2True, cat("CoveoFilesystem::FileListingOptions::Recursive")},
             {argument2False, cat("CoveoFilesystem::FileListingOptions::None")},
             {argument2, cat(node(argument2), " ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None")}});
    }

    auto createArgument3Matcher()
    {
        return hasArgument(3, expr().bind(argument3));
    }

    auto createArgument3Edit()
    {
        return cat(node(argument3));
    }

    auto createEditGeneral()
    {
        return changeTo(createArgument0Edit(
            cat(newMethod, "(", maybeDeref(object), ", ", createArgument1Edit(), ", ", createArgument2Edit(), ", ", createArgument3Edit(), ")")));
    }

    auto createEdit1Default()
    {
        const auto argument2Edit = selectBound(
            {{argument2True, cat(", CoveoFilesystem::FileListingOptions::Recursive")},
             {argument2False, cat("")},
             {argument2, cat(", ", node(argument2), " ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None")}});
        return changeTo(createArgument0Edit(cat(newMethod, "(", maybeDeref(object), ", ", createArgument1Edit(), argument2Edit, ")")));
    }

    auto createEdit2Defaults()
    {
        return changeTo(createArgument0Edit(cat(newMethod, "(", maybeDeref(object), ", ", createArgument1Edit(), ")")));
    }

    auto createEdit3Defaults()
    {
        return changeTo(createArgument0Edit(cat(newMethod, "(", maybeDeref(object), ")")));
    }

    auto createMatcherGeneral()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource,
                        cxxMemberCallExpr(on(expr().bind(object)),
                                          callee(functionDecl(hasName(oldMethod))),
                                          createArgument0Matcher(),
                                          createArgument1Matcher(),
                                          createArgument2Matcher(),
                                          createArgument3Matcher()));
    }

    auto createMatcher1Default()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource,
                        cxxMemberCallExpr(on(expr().bind(object)),
                                          callee(functionDecl(hasName(oldMethod))),
                                          createArgument0Matcher(),
                                          createArgument1Matcher(),
                                          createArgument2Matcher()));
    }

    auto createMatcher2Defaults()
    {
        return traverse(
            clang::TK_IgnoreUnlessSpelledInSource,
            cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher(), createArgument1Matcher()));
    }

    auto createMatcher3Defaults()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource,
                        cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher()));
    }

public:
    GetFileListRuleFactory() = default;

    auto createRuleGeneral()
    {
        return makeRule(createMatcherGeneral(), createEditGeneral(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRule1Default()
    {
        return makeRule(createMatcher1Default(), createEdit1Default(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRule2Defaults()
    {
        return makeRule(createMatcher2Defaults(), createEdit2Defaults(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRule3Defaults()
    {
        return makeRule(createMatcher3Defaults(), createEdit3Defaults(), createDeprecatedMessage(oldMethod, newMethod));
    }
};

auto createRuleDeleteFileList()
{
    const auto        oldMethod             = "::CGLFile::Path::DeleteFileList";
    const auto        newMethod             = "CoveoFilesystem::DeleteFiles";
    const std::string object                = "object";
    const std::string argument              = "argument";
    const std::string argumentStringLiteral = "argumentStringLiteral";

    const auto argumentMatcher = hasArgument(0, anyOf(stringLiteral().bind(argumentStringLiteral), expr().bind(argument)));
    const auto argumentEdit    = selectBound({{argumentStringLiteral, u16ToCharStringLiteral(cat(node(argumentStringLiteral)))},
                                              {argument, cat("CGLCharset::Conv::ConvertU16ToU8StringCopy(", node(argument), ")")}});

    auto rule = makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), argumentMatcher)),
        changeTo(cat(newMethod, "(", maybeDeref(object), ", ", argumentEdit, ")")),
        createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

class MethodsMoveRuleFactory
{
    const char* oldMethod = nullptr;
    const char* newMethod = "CoveoFilesystem::MoveFile";

    const std::string object        = "object";
    const std::string argument0     = "argument0";
    const std::string argument1     = "argument1";
    const std::string argumentFalse = "argumentFalse";
    const std::string argumentTrue  = "argumentTrue";

    auto createArgument0Matcher()
    {
        return hasArgument(0, expr().bind(argument0));
    }

    auto createArgument1Matcher()
    {
        return hasArgument(1,
                           anyOf(cxxBoolLiteral(equals(false)).bind(argumentFalse), cxxBoolLiteral(equals(true)).bind(argumentTrue), expr().bind(argument1)));
    }

    auto createArgument0Edit()
    {
        return access(argument0, "u16string()");
    }

    auto createArgument1Edit()
    {
        return selectBound(
            {{argumentTrue, cat(", std::filesystem::copy_options::overwrite_existing")},
             {argumentFalse, cat("")},
             {argument1, cat(", ", node(argument1), " ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none")}});
    }

    auto createMatcher()
    {
        return traverse(
            clang::TK_IgnoreUnlessSpelledInSource,
            cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher(), createArgument1Matcher()));
    }

    auto createMatcherDefault()
    {
        return traverse(clang::TK_IgnoreUnlessSpelledInSource,
                        cxxMemberCallExpr(on(expr().bind(object)), callee(functionDecl(hasName(oldMethod))), createArgument0Matcher()));
    }

    auto createOptionalEditForMove()
    {
        if (std::string{oldMethod} != "::CGLFile::Path::Move") {
            return cat();
        }
        return cat(maybeDeref(object), " = ");
    }

    auto createEdit()
    {
        return changeTo(
            cat(createOptionalEditForMove(), newMethod, "(", access(object, "u16string()"), ", ", createArgument0Edit(), createArgument1Edit(), ")"));
    }

    auto createEditDefault()
    {
        return changeTo(cat(createOptionalEditForMove(), newMethod, "(", access(object, "u16string()"), ", ", createArgument0Edit(), ")"));
    }

public:
    MethodsMoveRuleFactory(const char* oldMethodRhs) : oldMethod{oldMethodRhs}
    {
    }

    auto createRule()
    {
        return makeRule(createMatcher(), createEdit(), createDeprecatedMessage(oldMethod, newMethod));
    }

    auto createRuleDefault()
    {
        return makeRule(createMatcherDefault(), createEditDefault(), createDeprecatedMessage(oldMethod, newMethod));
    }
};

auto createRuleGetAvailableDiskSpace()
{
    const auto oldMethod = "::CGLFile::Path::GetAvailableDiskSpace";
    const auto newMethod = "space";

    const auto object = "object";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxMemberCallExpr(on(expr().bind(object)), callee(cxxMethodDecl(hasName(oldMethod))))),
                    changeTo(cat(newMethod, "(", maybeDeref(object), ").available")),
                    createDeprecatedMessage(oldMethod, newMethod));
}

auto createRuleGetTotalDiskSpace()
{
    const auto oldMethod = "::CGLFile::Path::GetTotalDiskSpace";
    const auto newMethod = "space";

    const auto object = "object";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, cxxMemberCallExpr(on(expr().bind(object)), callee(cxxMethodDecl(hasName(oldMethod))))),
                    changeTo(cat(newMethod, "(", maybeDeref(object), ").capacity")),
                    createDeprecatedMessage(oldMethod, newMethod));
}

auto createRuleGetDiskSpaceStats()
{
    const auto oldMethod = "::CGLFile::Path::GetDiskSpaceStats";
    const auto newMethod = "space";

    const auto object    = "object";
    const auto argument0 = "argument0";
    const auto argument1 = "argument1";
    return makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                             cxxMemberCallExpr(on(expr().bind(object)),
                                               callee(cxxMethodDecl(hasName(oldMethod))),
                                               hasArgument(0, anyOf(expr(unless(anyOf(cxxNullPtrLiteralExpr(), integerLiteral()))).bind(argument0), expr())),
                                               hasArgument(1, anyOf(expr(unless(anyOf(cxxNullPtrLiteralExpr(), integerLiteral()))).bind(argument1), expr())))),
                    changeTo(cat("{\nconst std::filesystem::space_info tmp = ",
                                 newMethod,
                                 "(",
                                 maybeDeref(object),
                                 ");\n",
                                 ifBound(argument0, cat(maybeDeref(argument0), " = tmp.available;\n"), cat()),
                                 ifBound(argument1, cat(maybeDeref(argument1), " = tmp.capacity;\n"), cat()),
                                 "}")),
                    createDeprecatedMessage(oldMethod, newMethod));
}

auto createRuleCurrentPathWithArg()
{
    const auto oldMethod = "::CGLFile::Path::CurrentPath";
    const auto newMethod = "current_path";
    const auto argument0 = "argument0";
    auto       rule =
        makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument0)))),
                 changeTo(cat(newMethod, "(", node(argument0), ")")),
                 createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleCurrentPathNoArg()
{
    const auto oldMethod = "::CGLFile::Path::CurrentPath";
    const auto newMethod = "std::filesystem::current_path";
    auto       rule      = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName(oldMethod))))),
                         changeTo(cat("CGLFile::Path{", newMethod, "()}")),
                         createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleTempPath()
{
    const auto oldMethod = "::CGLFile::Path::TempPath";
    const auto newMethod = "std::filesystem::temp_directory_path";
    auto       rule      = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName(oldMethod))))),
                         changeTo(cat("CGLFile::Path{", newMethod, "()}")),
                         createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleTempFileName()
{
    const auto oldMethod              = "::CGLFile::Path::TempFileName";
    const auto newMethod              = "CoveoFilesystem::CreateTempFile";
    const auto argument0              = "argument0";
    const auto argument0StringLiteral = "argument0StringLiteral";
    const auto argument1              = "argument1";
    const auto argument1StringLiteral = "argument1StringLiteral";
    const auto argument2              = "argument2";
    const auto argument2StringLiteral = "argument2StringLiteral";

    const auto argument0Matcher = hasArgument(0, anyOf(stringLiteral().bind(argument0StringLiteral), expr().bind(argument0)));
    const auto argument0Edit =
        selectBound({{argument0StringLiteral, cat("CGLFile::Path{", node(argument0StringLiteral), "}")}, {argument0, cat(node(argument0))}});
    const auto argument1Matcher = hasArgument(1, anyOf(stringLiteral().bind(argument1StringLiteral), expr().bind(argument1)));
    const auto argument1Edit    = selectBound({{argument1StringLiteral, u16ToCharStringLiteral(cat(node(argument1StringLiteral)))},
                                               {argument1, cat("CGLCharset::Conv::ConvertU16ToU8StringCopy(", node(argument1), ")")}});
    const auto argument2Matcher = hasArgument(2, anyOf(stringLiteral().bind(argument2StringLiteral), expr().bind(argument2)));
    const auto argument2Edit    = selectBound({{argument2StringLiteral, u16ToCharStringLiteral(cat(node(argument2StringLiteral)))},
                                               {argument2, cat("CGLCharset::Conv::ConvertU16ToU8StringCopy(", node(argument2), ")")}});

    auto rule = makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                                  callExpr(callee(functionDecl(hasName(oldMethod))), argument0Matcher, argument1Matcher, argument2Matcher)),
                         changeTo(cat(newMethod, "(", argument0Edit, ", ", argument1Edit, ", ", argument2Edit, ")")),
                         createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleToLowerCase()
{
    const auto oldMethod = "::CGLFile::Path::ToLowerCase";
    const auto newMethod = "CGL::Unicode::ToLowerCaseCopy";
    const auto argument0 = "argument0";
    const auto argument1 = "argument1";
    auto       rule =
        makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource,
                          callExpr(callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument0)), hasArgument(1, expr().bind(argument1)))),
                 changeTo(cat(node(argument1), " = ", newMethod, "(", node(argument0), ")")),
                 createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createRuleToLowerCaseInPlace()
{
    const auto oldMethod = "::CGLFile::Path::ToLowerCaseInPlace";
    const auto newMethod = "CGL::Unicode::ToLowerCaseCopy";
    const auto argument0 = "argument0";
    auto       rule =
        makeRule(traverse(clang::TK_IgnoreUnlessSpelledInSource, callExpr(callee(functionDecl(hasName(oldMethod))), hasArgument(0, expr().bind(argument0)))),
                 changeTo(cat(node(argument0), " = ", newMethod, "(", node(argument0), ")")),
                 createDeprecatedMessage(oldMethod, newMethod));
    return rule;
}

auto createReplaceCGLPathByStdRule()
{
    return applyFirst(merge({createRulesForMethodsWithNoArg(),
                             createRulesForMethodsWithNoArgToFunction(),
                             createRulesForMethodsOneArgToOperator(),
                             createRulesForMethodsWithOneArgToFunctionWithThisAtTheEnd(),

                             {
                                 createRuleEquivalent(),

                                 createRuleChangeExtensionAppend(),
                                 createRuleChangeExtensionReplaceAll(),
                                 createRuleChangeExtensionDefaultArg(),

                                 createRuleContains(),
                                 createRuleReadSymbolicLink(),

                                 MethodsCopyRuleFactory{"::CGLFile::Path::RecursiveCopy", "copy"}.createRule(),
                                 MethodsCopyRuleFactory{"::CGLFile::Path::RecursiveCopy", "copy"}.createRuleDefault(),
                                 MethodsCopyRuleFactory{"::CGLFile::Path::Copy", "copy_file"}.createRule(),
                                 MethodsCopyRuleFactory{"::CGLFile::Path::Copy", "copy_file"}.createRuleDefault(),

                                 GetFileListSizeRuleFactory{}.createRuleGeneral(),
                                 GetFileListSizeRuleFactory{}.createRule1Default(),
                                 GetFileListSizeRuleFactory{}.createRule2Defaults(),

                                 GetFileListRuleFactory{}.createRuleGeneral(),
                                 GetFileListRuleFactory{}.createRule1Default(),
                                 GetFileListRuleFactory{}.createRule2Defaults(),
                                 GetFileListRuleFactory{}.createRule3Defaults(),

                                 createRuleDeleteFileList(),

                                 MethodsMoveRuleFactory{"::CGLFile::Path::MoveFileOnly"}.createRule(),
                                 MethodsMoveRuleFactory{"::CGLFile::Path::MoveFileOnly"}.createRuleDefault(),
                                 MethodsMoveRuleFactory{"::CGLFile::Path::Move"}.createRule(),
                                 MethodsMoveRuleFactory{"::CGLFile::Path::Move"}.createRuleDefault(),

                                 createRuleGetAvailableDiskSpace(),
                                 createRuleGetTotalDiskSpace(),
                                 createRuleGetDiskSpaceStats(),

                                 createRuleCurrentPathWithArg(),
                                 createRuleCurrentPathNoArg(),
                                 createRuleTempPath(),
                                 createRuleTempFileName(),

                                 createRuleToLowerCase(),
                                 createRuleToLowerCaseInPlace(),
                             }}));
}

// Boilerplate

class ReplaceCGLPathByStdCheck : public TransformerClangTidyCheck
{
public:
    ReplaceCGLPathByStdCheck(StringRef Name, ClangTidyContext* Context) : TransformerClangTidyCheck(createReplaceCGLPathByStdRule(), Name, Context)
    {
    }
};

class ReplaceCGLPathByStdCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<ReplaceCGLPathByStdCheck>("coveo-replacecglpathbystdcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::ReplaceCGLPathByStdCheckModule> replaceCGLPathByStdCheckInit("coveo-replacecglpathbystdcheck-module",
                                                                                                   "Adds 'coveo-replacecglpathbystdcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int replaceCGLPathByStdCheckAnchorSource = 0;

}  // namespace clang::tidy
