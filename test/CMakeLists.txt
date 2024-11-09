cmake_minimum_required(VERSION 3.10)

set(test A_Language_tests)

set(test_sources 
    lexer_test.cpp
)

add_executable(${test} ${test_sources})

target_link_libraries(${test} PUBLIC 
                      gtest_main
                      A_Language_lib
)

add_test(NAME ${test} COMMAND ${test})

# set the output directory for the test binary
set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${MAINFOLDER}/test")
