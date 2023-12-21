#include <string>

#include "clang/Tooling/Transformer/RewriteRule.h"  // RewriteRuleWith

namespace AutoPtrClangTidyUtils {

::clang::transformer::RewriteRuleWith<std::string> createRemoveIsNullRule(const std::string& parentClass);
::clang::transformer::RewriteRuleWith<std::string> createUseStdMoveForReferenceConstructor(const std::string& parentClass);
::clang::transformer::RewriteRuleWith<std::string> createReplacePtrAssignmentRule(const std::string& parentClass);

}  // namespace AutoPtrClangTidyUtils