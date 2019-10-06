cmake_minimum_required(VERSION 3.10.2)
project(ini_loader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

add_executable(ini_loader ini.cpp ini.h test_ini.cpp)