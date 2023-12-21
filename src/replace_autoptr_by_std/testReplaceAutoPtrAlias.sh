#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
if [[ "${TRACE-0}" == "1" ]]; then
    set -o xtrace
fi

buildDirectory=${1-build}
updateExpected=${2-0}


SCRIPT_DIRECTORY="$(cd "$(dirname "$0")"; pwd -P)"
source "$SCRIPT_DIRECTORY/../../testFramework.sh"


testClangTidyReplacement \
    "$buildDirectory" \
    "$updateExpected" \
    "$SCRIPT_DIRECTORY/TestCallsReplacementsAlias.cpp" \
    "$SCRIPT_DIRECTORY/ExpectedTestCallsReplacementsNoAlias.cpp" \
    "coveo-replaceautoptraliasdefcheck" \
    "libReplaceAutoPtrAliasDefCheck.so"
