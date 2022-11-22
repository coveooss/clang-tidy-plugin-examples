# clang-tidy plugins

This folder contains [clang-tidy plugins](https://reviews.llvm.org/D111100).

## Quick Start

To use the plugins,

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
    If you do not want to build `llvm`, you can follow the approach of [testcpp.yml](../.github/workflows/testcpp.yml) instead.
1. You can then build the `coveo/devtools/clang-tidy-plugins` project
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
            --load $SRC_COVEO/devtools/clang-tidy-plugins/build/lib/libAwesomePrefixCheck.so \
            --list-checks \
        | grep awesomeprefixcheck
    ```
1. To use your plugin, you need to compile your code with the same clang version used to run the plugin, and to export the compile database with `CMAKE_EXPORT_COMPILE_COMMANDS=ON`
    ```shell
    cd $CPP_COVEO/cglbaselib
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
        --load $SRC_COVEO/devtools/clang-tidy-plugins/build/lib/libAwesomePrefixCheck.so \
        -p $CPP_COVEO/cglbaselib/debug/compile_commands.json \
        $CPP_COVEO/cglbaselib/CGLBaseLib/CGLBase/src/CGLBinaryStreamBuffer.cpp
    ```

## References

- [Writing clang-tidy checks](https://clang.llvm.org/extra/clang-tidy/Contributing.html#out-of-tree-check-plugins)
- [Stephen Kelly - Refactor your codebase with Clang tooling](https://www.youtube.com/watch?v=_T-5pWQVxeE)
- [Stephen Kelly - The Future of AST Matcher-based Refactoring](https://www.youtube.com/watch?v=yqi8U8Q0h2g)
- [Stephen Kelly Blog](https://steveire.wordpress.com/)

Every Stephen Kelly references were written before that `clang-tidy` introduce plugins.
