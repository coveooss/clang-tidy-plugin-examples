#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
if [[ "${TRACE-0}" == "1" ]]; then
    set -o xtrace
fi

buildDirectory=${1-build}

SCRIPT_DIRECTORY="$(dirname "$(readlink --canonicalize "$0")")"


"$SCRIPT_DIRECTORY/src/replace_cglpath_by_std/testReplacePath.sh" "$buildDirectory"

"$SCRIPT_DIRECTORY/src/replace_autoptr_by_std/testReplaceAutoPtrAlias.sh" "$buildDirectory"

"$SCRIPT_DIRECTORY/src/replace_smartptr_by_std/testReplaceSmartPtr.sh" "$buildDirectory"
