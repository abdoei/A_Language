cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyLang VERSION 0.1)

# Project Output Paths
set(MAINFOLDER ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MAINFOLDER}/bin")
set(LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CC "gcc")
set(CXX "g++")

# Add the include directory
include_directories(inc src)

# Add the src subdirectory
add_subdirectory(src)

# Add the GTest directory
add_subdirectory(external/gtest)

# Enable testing if requested
enable_testing()
add_subdirectory(test)
