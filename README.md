# clang-tidy plugins

This folder contains [clang-tidy plugins](https://reviews.llvm.org/D111100).

## Quick Start

To use the plugins, you need at least the version 14.0 of LLVM/Clang.
There are 3 alternate ways you can proceed.

### 1. Installing clang

1. On Ubuntu, you can install LLVM/Clang version 15 with the script [installClang15.sh](./installClang15.sh)
    ```shell
    ./installClang15.sh
    ```
1. Build the `clang-tidy-plugin-example` project
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
    cmake \
        -D CMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -B buildRepoUsingPlugin -S repo-using-plugin
    cmake --build buildRepoUsingPlugin
    ```
1. That's it, you can now use the plugin!
    ```shell
    > clang-tidy \
        --checks='awesomeprefixcheck' \
        --load build/lib/libAwesomePrefixCheck.so \
        -p buildRepoUsingPlugin/compile_commands.json \
        repo-using-plugin/src/code.cpp

    1 warning generated.
    /home/klalumiere/src/coveo/clang-tidy-plugin-example/repo-using-plugin/src/code.cpp:1:5: warning: function 'f' is insufficiently awesome [awesomeprefixcheck]
    int f() { return 0; }
        ^
    /home/klalumiere/src/coveo/clang-tidy-plugin-example/repo-using-plugin/src/code.cpp:1:5: note: insert 'awesome'
    int f() { return 0; }
        ^
        awesome_
    ```

### 2. Using a container image

See [testcpp.yml](.github/workflows/testcpp.yml).

### 3. Building clang

1. Build a recent version of LLVM/Clang (14.0 at least)
    ```shell
    cd $HOME/src
    git clone https://github.com/llvm/llvm-project
    cd llvm-project
    git checkout 4af73d7ebb5bf4bc2045d15ae0f4ebce56f2d86f # optional
    cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' -B build -S llvm
    cmake --build build
    cmake --install build --prefix staging
    ```
1. Build the `clang-tidy-plugin-example` project
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
    CC=$HOME/src/llvm-project/staging/bin/clang \
    CXX=$HOME/src/llvm-project/staging/bin/clang++ \
        cmake \
            -D CMAKE_EXPORT_COMPILE_COMMANDS=ON \
            -B buildRepoUsingPlugin -S repo-using-plugin
    cmake --build buildRepoUsingPlugin
    ```
1. That's it, you can now use the plugin!
    ```shell
    > $HOME/src/llvm-project/staging/bin/clang-tidy \
        --checks='awesomeprefixcheck' \
        --load build/lib/libAwesomePrefixCheck.so \
        -p buildRepoUsingPlugin/compile_commands.json \
        repo-using-plugin/src/code.cpp

    1 warning generated.
    /home/klalumiere/src/coveo/clang-tidy-plugin-example/repo-using-plugin/src/code.cpp:1:5: warning: function 'f' is insufficiently awesome [awesomeprefixcheck]
    int f() { return 0; }
        ^
    /home/klalumiere/src/coveo/clang-tidy-plugin-example/repo-using-plugin/src/code.cpp:1:5: note: insert 'awesome'
    int f() { return 0; }
    ```

## References

- [Writing clang-tidy checks](https://clang.llvm.org/extra/clang-tidy/Contributing.html#out-of-tree-check-plugins)
- [Stephen Kelly - Refactor your codebase with Clang tooling](https://www.youtube.com/watch?v=_T-5pWQVxeE)
- [Stephen Kelly - The Future of AST Matcher-based Refactoring](https://www.youtube.com/watch?v=yqi8U8Q0h2g)
- [Stephen Kelly Blog](https://steveire.wordpress.com/)

Every Stephen Kelly references were written before that `clang-tidy` introduce plugins.
