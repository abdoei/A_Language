add_executable(test1 lexer_test.cpp)

target_link_libraries(test1 gtest gtest_main)

add_test(NAME test1 COMMAND test1)

unset(TESTING CACHE)

# set the output directory for the test binary
set_target_properties(test1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${MAINFOLDER}/test")
