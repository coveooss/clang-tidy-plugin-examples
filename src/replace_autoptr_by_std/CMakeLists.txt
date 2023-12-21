add_library(AutoPtrShared SHARED "")
target_include_directories(AutoPtrShared
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(AutoPtrShared PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/AutoPtrShared.h
    ${CMAKE_CURRENT_LIST_DIR}/AutoPtrShared.cpp
)

add_library(ReplaceAutoPtrByStdCheck MODULE "")
target_compile_options(ReplaceAutoPtrByStdCheck PRIVATE -fno-rtti)
target_link_libraries(ReplaceAutoPtrByStdCheck PRIVATE AutoPtrShared)
target_include_directories(ReplaceAutoPtrByStdCheck
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(ReplaceAutoPtrByStdCheck PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/ReplaceAutoPtrByStdCheck.cpp
)

add_library(ReplaceArrayAutoPtrByStdCheck MODULE "")
target_compile_options(ReplaceArrayAutoPtrByStdCheck PRIVATE -fno-rtti)
target_link_libraries(ReplaceArrayAutoPtrByStdCheck PRIVATE AutoPtrShared)
target_include_directories(ReplaceArrayAutoPtrByStdCheck
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(ReplaceArrayAutoPtrByStdCheck PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/ReplaceArrayAutoPtrByStdCheck.cpp
)

add_library(ReplaceAutoPtrAliasDefCheck MODULE "")
target_compile_options(ReplaceAutoPtrAliasDefCheck PRIVATE -fno-rtti)
target_include_directories(ReplaceAutoPtrAliasDefCheck
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(ReplaceAutoPtrAliasDefCheck PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/ReplaceAutoPtrAliasDefCheck.cpp
)


add_library(TestCallsReplacementsAutoPtr SHARED "")
target_sources(TestCallsReplacementsAutoPtr PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CGLAutoPtrStub.h

    ${CMAKE_CURRENT_LIST_DIR}/TestCallsReplacementsAutoPtr.cpp
)

add_library(TestCallsReplacementsArrayAutoPtr SHARED "")
target_sources(TestCallsReplacementsArrayAutoPtr PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CGLArrayAutoPtrStub.h

    ${CMAKE_CURRENT_LIST_DIR}/TestCallsReplacementsArrayAutoPtr.cpp
)

add_library(TestCallsReplacementsAlias SHARED "")
target_compile_definitions(TestCallsReplacementsAlias PRIVATE INCLUDE_PTR_ALIAS_STUBS)
target_sources(TestCallsReplacementsAlias PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/TestCallsReplacementsAlias.cpp
)

add_library(TestCallsReplacementsAliasWithStdWithStd SHARED "")
target_sources(TestCallsReplacementsAliasWithStdWithStd PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ExpectedTestCallsReplacementsNoAlias.cpp
)
