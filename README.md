# clang-tidy plugins

This folder contains [clang-tidy plugins](https://reviews.llvm.org/D111100).

## Quick Start

To use the plugins, you need at least the version 14.0 of LLVM/Clang.

### Installing clang

1. On Ubuntu, you can install LLVM/Clang version 15 with
    ```shell
    export DEBIAN_FRONTEND=noninteractive
        sudo apt update
        sudo --preserve-env apt-get install --assume-yes --no-install-recommends curl gpg
        mkdir --parents /usr/share/keyrings/
        curl --fail --location --show-error --silent https://apt.llvm.org/llvm-snapshot.gpg.key \
            | gpg --dearmor - \
            | sudo tee /usr/share/keyrings/llvm-toolchain-15.gpg > /dev/null
        echo "deb [arch=amd64 signed-by=/usr/share/keyrings/llvm-toolchain-15.gpg] http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-15 main
    deb-src [arch=amd64 signed-by=/usr/share/keyrings/llvm-toolchain-15.gpg] http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-15 main" \
            | sudo tee /etc/apt/sources.list.d/llvm-toolchain-15.list
        sudo apt update
        sudo --preserve-env apt install --assume-yes \
            clang-15 \
            clang-format-15 \
            clang-tidy-15 \
            libclang-15-dev \
            llvm-15-dev
        sudo update-alternatives --install /usr/local/bin/clang clang /usr/bin/clang-15 99
        sudo update-alternatives --install /usr/local/bin/clang++ clang++ /usr/bin/clang++-15 99
        sudo update-alternatives --install /usr/local/bin/clang-format clang-format /usr/bin/clang-format-15 99
        sudo update-alternatives --install /usr/local/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-15 99
    ```
1. You can then build the `clang-tidy-plugin-example` project
    ```shell
    cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
    cmake --build build
    ```
1. You can now list your plugin
    ```shell
    clang-tidy \
            --checks='*' \
            --load build/lib/libAwesomePrefixCheck.so \
            --list-checks \
        | grep awesomeprefixcheck
    ```
1. To use your plugin, you need to export the compile database with `CMAKE_EXPORT_COMPILE_COMMANDS=ON`
    ```shell
    cd $HOME/src/repo-using-plugin
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build -S .
    cmake --build build
    ```
1. That's it, you can now use the plugin!
    ```shell
    clang-tidy \
        --checks='awesomeprefixcheck' \
        --load $HOME/src/clang-tidy-plugin-example/build/lib/libAwesomePrefixCheck.so \
        -p build/compile_commands.json \
        src/File.cpp
    ```

### Using a container image

See the approach used in [testcpp.yml](.github/workflows/testcpp.yml).

### Building clang

If you feel adventurous,

1. Get a recent version of LLVM/Clang (14.0 at least)
    ```shell
    cd $HOME/src
    git clone https://github.com/llvm/llvm-project
    cd llvm-project
    git checkout 178674e23a7129fd8b10d72a5f9722fb876f8c15 # optional
    cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' -B build -S llvm
    cmake --build build
    cmake --install build --prefix staging
    ```
1. You can then build the `clang-tidy-plugin-example` project
    ```shell
    cmake \
        -DClang_DIR=$HOME/src/llvm-project/staging/lib/cmake/clang \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_DIR=$HOME/src/llvm-project/staging/lib/cmake/llvm \
        -B build -S .
    cmake --build build
    ```
1. You can now list your plugin
    ```shell
    $HOME/src/llvm-project/staging/bin/clang-tidy \
            --checks='*' \
            --load build/lib/libAwesomePrefixCheck.so \
            --list-checks \
        | grep awesomeprefixcheck
    ```
1. To use your plugin, you need to compile your code with the same clang version used to run the plugin, and to export the compile database with `CMAKE_EXPORT_COMPILE_COMMANDS=ON`
    ```shell
    cd $HOME/src/repo-using-plugin
    CC=$HOME/src/llvm-project/staging/bin/clang \
    CXX=$HOME/src/llvm-project/staging/bin/clang++ \
        cmake \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
            -B debug -S .
    cmake --build debug
    ```
1. That's it, you can now use the plugin!
    ```shell
    $HOME/src/llvm-project/staging/bin/clang-tidy \
        --checks='awesomeprefixcheck' \
        --load $HOME/src/clang-tidy-plugin-example/build/lib/libAwesomePrefixCheck.so \
        -p build/compile_commands.json \
        src/File.cpp
    ```

## References

- [Writing clang-tidy checks](https://clang.llvm.org/extra/clang-tidy/Contributing.html#out-of-tree-check-plugins)
- [Stephen Kelly - Refactor your codebase with Clang tooling](https://www.youtube.com/watch?v=_T-5pWQVxeE)
- [Stephen Kelly - The Future of AST Matcher-based Refactoring](https://www.youtube.com/watch?v=yqi8U8Q0h2g)
- [Stephen Kelly Blog](https://steveire.wordpress.com/)

Every Stephen Kelly references were written before that `clang-tidy` introduce plugins.
