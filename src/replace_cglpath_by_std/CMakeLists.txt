add_library(ReplaceCGLPathByStd MODULE "")
target_compile_options(ReplaceCGLPathByStd PRIVATE -fno-rtti)
target_include_directories(ReplaceCGLPathByStd
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(ReplaceCGLPathByStd PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ReplaceCGLPathByStd.cpp
)


add_library(TestCallsReplacementsPathStub SHARED "")
target_sources(TestCallsReplacementsPathStub PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CGLPathStub.h

    ${CMAKE_CURRENT_LIST_DIR}/TestCallsReplacementsPath.cpp
)

add_library(TestCallsReplacementsPathWithStd SHARED "")
target_compile_definitions(TestCallsReplacementsPathWithStd PRIVATE TEST_WITH_PATH_BEING_AN_ALIAS_OF_STD_FILESYSTEM)
target_sources(TestCallsReplacementsPathWithStd PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CGLPathIsStdfilesystemPath.h

    ${CMAKE_CURRENT_LIST_DIR}/ExpectedTestCallsReplacementsPath.cpp
)
