cmake_minimum_required(VERSION 3.10)
project(pipeline)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")# -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)

add_executable(pipeline main.cpp)
