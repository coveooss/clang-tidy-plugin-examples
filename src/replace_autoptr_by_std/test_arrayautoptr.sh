cmake --build ../../build && clang-tidy --fix --checks='-*,coveo-replacearrayautoptrbystdcheck' --load=../../build/lib/libReplaceArrayAutoPtrByStdCheck.so -p ../../build/compile_commands.json TestCallsReplacementsArrayAutoPtr.cpp