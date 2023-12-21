#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
if [[ "${TRACE-0}" == "1" ]]; then
    set -o xtrace
fi

export DEBIAN_FRONTEND=noninteractive
sudo apt update
sudo --preserve-env apt-get install --assume-yes --no-install-recommends curl gpg lsb-release
ubuntuCodename=$(lsb_release --codename --short)
mkdir --parents /usr/share/keyrings/
curl --fail --location --show-error --silent https://apt.llvm.org/llvm-snapshot.gpg.key \
    | gpg --dearmor - \
    | sudo tee /usr/share/keyrings/llvm-toolchain-16.gpg > /dev/null
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/llvm-toolchain-16.gpg] http://apt.llvm.org/$ubuntuCodename/ llvm-toolchain-$ubuntuCodename-16 main
deb-src [arch=amd64 signed-by=/usr/share/keyrings/llvm-toolchain-16.gpg] http://apt.llvm.org/$ubuntuCodename/ llvm-toolchain-$ubuntuCodename-16 main" \
    | sudo tee /etc/apt/sources.list.d/llvm-toolchain-16.list
sudo apt update
sudo --preserve-env apt install --assume-yes \
    clang-16 \
    clang-format-16 \
    clang-tidy-16 \
    libclang-16-dev \
    llvm-16-dev
