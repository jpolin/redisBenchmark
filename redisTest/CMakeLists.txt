cmake_minimum_required(VERSION 2.6)

### Name the project
project(redisTest)

### Make sure the generated makefile is not shortened
SET(CMAKE_VERBOSE_MAKEFILE ON)

### Include Directories
SET(BASE_DIR /home/jpolin/git/redisBenchmark)
SET(REDOX_DIR /home/jpolin/git/redox)

## Includes
INCLUDE_DIRECTORIES( ${BASE_DIR}/redistTest ${BASE_DIR}/test_functions ${REDOX_DIR}/include)

## Set the compilation flags
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -g")

## Exectuables
add_executable(redisTest ${BASE_DIR}/redisTest/redisPubSub.cpp
                         ${BASE_DIR}/test_functions/test_functions.cpp)

## Link Libraries
find_library(REDOX_LIB redox ${REDOX_DIR}/build)
target_link_libraries(redisTest ${REDOX_LIB} ev hiredis)
