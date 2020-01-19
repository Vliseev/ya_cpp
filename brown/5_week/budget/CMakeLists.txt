cmake_minimum_required(VERSION 3.15)
project(budget)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(budget main.cpp)