# clang-tidy plugins

This folder contains [clang-tidy plugins](https://reviews.llvm.org/D111100).

## Quick Start

To build the plugins, first, you'll need LLVM/Clang version 16.
On Ubuntu, you can install them with the script `installClang16.sh`.
Then,

```shell
CC=clang-16 CXX=clang++-16 cmake -B build -G Ninja -S .
cmake --build build
```

To use the plugin,

1. You can list the plugin with
    ```shell
    clang-tidy-16 \
            --checks='*' \
            --load build/lib/libAwesomePrefixCheck.so \
            --list-checks \
        | grep coveo-awesomeprefixcheck
    ```
1. To use the plugin, you need to compile the code you want to use the plugin on, and to export the compile database with `CMAKE_EXPORT_COMPILE_COMMANDS=ON`
    ```shell
    cmake -B buildRepoUsingPlugin -S repo-using-plugin \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build buildRepoUsingPlugin
    ```
1. That's it, you can now use the plugin!
    ```shell
    clang-tidy-16 \
        --checks='coveo-awesomeprefixcheck' \
        --load build/lib/libAwesomePrefixCheck.so \
        -p buildRepoUsingPlugin/compile_commands.json \
        repo-using-plugin/src/code.cpp
    ```

## How to iterate quickly

Writing a clang-tidy check, finding a file in another repository where the check would apply and running the check on that file can get tedious quickly.
Luckily for us, there is a better way.

Create a stub of the code that you want to replace, like in the file [src/replace_cglpath_by_std/CGLPathStub.h](./src/replace_cglpath_by_std/CGLPathStub.h).
Then, use the methods that you want to replace in a function, like in the file [src/replace_cglpath_by_std/TestCallsReplacementsPath.cpp](./src/replace_cglpath_by_std/TestCallsReplacementsPath.cpp).
Separates `const` uses from mutable uses to make sure you don't inadvertently remove constness.
Then, write your custom check, for instance, `coveo-replacecglpathbystdcheck`.
Write a file that contains the code that you expect to have after your check has run, like [src/replace_cglpath_by_std/ExpectedTestCallsReplacementsPath.cpp](./src/replace_cglpath_by_std/ExpectedTestCallsReplacementsPath.cpp).
Now, to test, create a script similar to [src/replace_cglpath_by_std/testReplacePath.sh](./src/replace_cglpath_by_std/testReplacePath.sh).
This script also has an option that allows you to update the expected file [ExpectedTestCallsReplacementsPath.cpp](./src/replace_cglpath_by_std/ExpectedTestCallsReplacementsPath.cpp) with
```shell
src/replace_cglpath_by_std/testReplacePath.sh build 1
```
where `build` is your build directory and `1` is the value that the variable `updateExpected` takes.
Note that if you don't want to update the expected file, the script has default values for its parameters, so you can just call
```shell
src/replace_cglpath_by_std/testReplacePath.sh
```
and it will look for a build directory `build` in your current working directory.
If you want to run clang-tidy manually, take a look inside the script [testReplacePath.sh](./src/replace_cglpath_by_std/testReplacePath.sh) to see how to do it.

As [ExpectedTestCallsReplacementsPath.cpp](./src/replace_cglpath_by_std/ExpectedTestCallsReplacementsPath.cpp) illustrates, you can sometimes go one step further and write an end to end test.
In that example, you can uncomment `TEST_WITH_PATH_BEING_AN_ALIAS_OF_STD_FILESYSTEM` so that the header [src/replace_cglpath_by_std/CGLPathIsStdfilesystemPath.h](./src/replace_cglpath_by_std/CGLPathIsStdfilesystemPath.h) is included instead of [CGLPathStub.h](./src/replace_cglpath_by_std/CGLPathStub.h).
As the header name suggests, `Path` is defined as `using Path = std::filesystem::path` in  [CGLPathIsStdfilesystemPath.h](./src/replace_cglpath_by_std/CGLPathIsStdfilesystemPath.h) instead of being stubbed.
This is the end goal of the check: replacing our in-house path implementation `CGLFile::Path` by the standard `std::filesystem::path`.
`TEST_WITH_PATH_BEING_AN_ALIAS_OF_STD_FILESYSTEM` allows testing just that.
Finally, to test automatically, compile [ExpectedTestCallsReplacementsPath.cpp](./src/replace_cglpath_by_std/ExpectedTestCallsReplacementsPath.cpp) as illustrated in [src/replace_cglpath_by_std/CMakeLists.txt](./src/replace_cglpath_by_std/CMakeLists.txt) with the target `TestCallsReplacementsPathWithStd`.

## References

- [Clang Transformer Tutorial](https://intel.github.io/llvm-docs/clang/ClangTransformerTutorial.html)
- [AST Matcher Reference](https://clang.llvm.org/docs/LibASTMatchersReference.html)
- [Writing clang-tidy checks](https://clang.llvm.org/extra/clang-tidy/Contributing.html#out-of-tree-check-plugins)
- [Stephen Kelly - Refactor your codebase with Clang tooling](https://www.youtube.com/watch?v=_T-5pWQVxeE)
- [Stephen Kelly - The Future of AST Matcher-based Refactoring](https://www.youtube.com/watch?v=yqi8U8Q0h2g)
- [Stephen Kelly Blog](https://steveire.wordpress.com/)

Every Stephen Kelly references above were written/filmed before that `clang-tidy` introduce plugins.
