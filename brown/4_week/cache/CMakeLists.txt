cmake_minimum_required(VERSION 3.14)
project(cache)

set(CMAKE_CXX_STANDARD 17)

add_executable(cache main.cpp Solution.cpp)
target_link_libraries(cache pthread)