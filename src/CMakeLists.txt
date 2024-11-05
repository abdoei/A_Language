cmake_minimum_required(VERSION 3.10)

# Set the project name
project(A_Language)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(sources 
    main.cpp
    lexer.cpp
)

# Add the executable
add_executable(A_Language ${sources})
