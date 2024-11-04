cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyLang VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CC "gcc")
set(CXX "g++")

# Add options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(ENABLE_TESTS "Enable testing" OFF)

# Add the include directory
include_directories(inc)

# Add the src subdirectory
add_subdirectory(src)

# Enable testing if requested
if(ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()