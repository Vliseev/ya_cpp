cmake_minimum_required(VERSION 3.15)
project(comment_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3  -fstandalone-debug")
set(CMAKE_CXX_STANDARD 17)

add_executable(comment_server main.cpp)