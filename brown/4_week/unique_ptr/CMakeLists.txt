cmake_minimum_required(VERSION 3.12)
project(unique_ptr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wreturn-type -Werror")

add_executable(unique_ptr unique_ptr.cpp)
