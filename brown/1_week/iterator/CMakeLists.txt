cmake_minimum_required(VERSION 3.12)
project(iterator)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_CXX_STANDARD 14)

add_executable(iterator main.cpp)