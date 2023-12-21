cmake --build ../../build

clang-tidy --fix --checks='-*,coveo-replaceautoptraliasdefcheck' --load=../../build/lib/libReplaceAutoPtrAliasDefCheck.so -p ../../build/compile_commands.json TestCallsReplacementsAlias.cpp