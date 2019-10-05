cmake_minimum_required(VERSION 3.12)
project(secondary_index)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -Wfatal-errors")
set(CMAKE_CXX_STANDARD 17)

add_executable(secondary_index main.cpp)