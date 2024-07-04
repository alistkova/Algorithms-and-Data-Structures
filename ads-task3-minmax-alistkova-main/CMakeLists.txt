cmake_minimum_required(VERSION 3.0)

project(minmaxqueue)						# Название проекта

set(SRC_LIST minmaxqueue.cpp)				# Установка переменной со списком исходников для исполняемого файла

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -gdwarf-4 -std=c++20")

add_executable(${PROJECT_NAME} ${SRC_LIST})	# Создает исполняемый файл с именем проекта

