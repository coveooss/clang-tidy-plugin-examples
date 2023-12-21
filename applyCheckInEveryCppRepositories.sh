#!/usr/bin/env bash

#
# Example use:
# ./applyCheckInEveryCppRepositories.sh \
#   build/lib/libReplaceByGetExecutablePathCheck.so \
#   coveo-replacebygetexecutablepathcheck \
#   $CPP_COVEO
#

checkLibraryPath="$1"
checkName="$2"
cppRepositoriesPath="$3"
CMAKE_BUILD_PARALLEL_LEVEL=${CMAKE_BUILD_PARALLEL_LEVEL:-'7'}
USAGE="$0 <checkLibraryPath> <checkName> <cppRepositoriesPath>"


set -o errexit
set -o nounset
set -o pipefail
if [[ "${TRACE-0}" == "1" ]]; then
    set -o xtrace
fi
if [[ $# -lt 3 ]]; then
  echo 1>&2 "$0: not enough arguments. Usage: '$USAGE'"
  exit 2
elif [[ $# -gt 3 ]]; then
  echo 1>&2 "$0: too many arguments. Usage: '$USAGE'"
  exit 2
fi

export CMAKE_EXPORT_COMPILE_COMMANDS=TRUE

isExistingDirectory() {
    [[ -d "$1" ]]
}


declare -a StringArray=(
    repository1
    repository2
)

for value in "${StringArray[@]}"; do
    pushd "$cppRepositoriesPath/$value"

    cmake -G Ninja -B build -S .
    cmake --build build
    run-clang-tidy-16 \
            -checks="-*,$checkName" \
            -fix \
            -header-filter ".*" \
            -j "$CMAKE_BUILD_PARALLEL_LEVEL" \
            -load "$checkLibraryPath" \
            -p build \
            > outputClangTidyRefactoring.txt \
        || echo "Non zero exit code expected: continuing"

    popd
done
