cmake_minimum_required(VERSION 3.0)

project(radixsort)

set(SRC_LIST radixsort.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -gdwarf-4 -std=c++20")

add_executable(${PROJECT_NAME} ${SRC_LIST})

