add_executable(lexer_test lexer_test.cpp)

target_link_libraries(lexer_test gtest gtest_main)

add_test(NAME lexer_test COMMAND lexer_test)

unset(TESTING CACHE)

# set the output directory for the test binary
set_target_properties(lexer_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${MAINFOLDER}/test")
