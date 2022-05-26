cmake_minimum_required(VERSION 3.21)
project(DSACPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(DSACPP Vector/Vector.h Vector/main.cpp List/listNode.h List/List.h)
add_executable(cache miscellaneous/cache.cpp)