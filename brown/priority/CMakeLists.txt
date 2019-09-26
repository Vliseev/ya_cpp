cmake_minimum_required(VERSION 3.10)
project(priority)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -Wall -Wpedantic  -Wfatal-errors")

add_executable(priority main.cpp)