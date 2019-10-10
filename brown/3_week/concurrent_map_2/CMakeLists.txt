cmake_minimum_required(VERSION 3.15)
project(concurrent_map_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_STANDARD 17)

add_executable(concurrent_map_2 concurrent_map_2.cpp)
target_link_libraries(concurrent_map_2 pthread)