add_library(ReplaceSmartPtrBySharedCheck MODULE "")
target_compile_options(ReplaceSmartPtrBySharedCheck PRIVATE -fno-rtti)
target_include_directories(ReplaceSmartPtrBySharedCheck
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(ReplaceSmartPtrBySharedCheck PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/ReplaceSmartPtrBySharedCheck.cpp
)

add_library(FindRawPtrAssignmentToSmartCheck MODULE "")
target_compile_options(FindRawPtrAssignmentToSmartCheck PRIVATE -fno-rtti)
target_include_directories(FindRawPtrAssignmentToSmartCheck
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(FindRawPtrAssignmentToSmartCheck PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/FindRawPtrAssignmentToSmartCheck.cpp
)

add_library(TestCallsReplacementsSmartPtrStub SHARED "")
target_compile_definitions(TestCallsReplacementsSmartPtrStub PRIVATE INCLUDE_SMARTPTR_ALIAS_STUBS)
target_sources(TestCallsReplacementsSmartPtrStub PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CGLSmartPtrIsShared.h
    ${CMAKE_CURRENT_LIST_DIR}/CGLSmartPtrStub.h

    ${CMAKE_CURRENT_LIST_DIR}/TestCallsReplacementsSmartPtr.cpp
)

add_library(TestFindRawPtrAssignmentToSmart SHARED "")
target_sources(TestFindRawPtrAssignmentToSmart PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CGLSmartPtrStub.h

    ${CMAKE_CURRENT_LIST_DIR}/TestFindRawPtrAssignmentToSmart.cpp
)

add_library(TestCallsReplacementsSmartPtrWithStd SHARED "")
target_sources(TestCallsReplacementsSmartPtrWithStd PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ExpectedTestCallsReplacementsSmartPtr.cpp
)

