cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# Sets the `variable` to `value` if the `variable` is not already defined
function(set_if_not_defined variable value)
    if(NOT DEFINED ${variable})
        set(${variable} "${value}" PARENT_SCOPE)
    endif()
endfunction()


project(clang-tidy-plugins LANGUAGES C CXX)

option(BUILD_WARNINGS "Build with project's warnings setting" ON)
option(BUILD_WARNING_AS_ERROR "Build with warning as error" ON)
if(BUILD_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        add_compile_options(-Wall)
    endif()
endif()
if(BUILD_WARNING_AS_ERROR)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        add_compile_options(-Werror)
    endif()
endif()

include(GNUInstallDirs)
set_if_not_defined(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set_if_not_defined(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set_if_not_defined(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

enable_testing()
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
set_if_not_defined(CMAKE_CXX_STANDARD 20)


add_subdirectory(src)
