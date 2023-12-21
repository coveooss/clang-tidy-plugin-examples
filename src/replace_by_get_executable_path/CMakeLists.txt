add_library(ReplaceByGetExecutablePathCheck MODULE "")
target_compile_options(ReplaceByGetExecutablePathCheck PRIVATE -fno-rtti)
target_include_directories(ReplaceByGetExecutablePathCheck
    PRIVATE
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(ReplaceByGetExecutablePathCheck PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ReplaceByGetExecutablePathCheck.cpp)
