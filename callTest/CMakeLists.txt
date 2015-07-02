cmake_minimum_required(VERSION 2.6)

### Name the project
project(callTest)

### Make sure the generated makefile is not shortened
SET(CMAKE_VERBOSE_MAKEFILE ON)

### Include Directories
SET(BASE_DIR /home/jpolin/git/redisBenchmark)

## Includes
INCLUDE_DIRECTORIES( ${BASE_DIR}/redistTest ${BASE_DIR}/test_functions)

## Set the compilation flags
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -g")

## Exectuables
add_executable(callTest ${BASE_DIR}/callTest/callTest.cpp
                         ${BASE_DIR}/test_functions/test_functions.cpp)
