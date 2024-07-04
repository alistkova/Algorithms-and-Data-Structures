cmake_minimum_required(VERSION 3.0)
project(battle)

set(SRC_LIST battle.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -g")

add_executable(${PROJECT_NAME} ${SRC_LIST})

