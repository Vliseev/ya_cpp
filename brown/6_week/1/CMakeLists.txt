cmake_minimum_required(VERSION 3.14)
project(1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -fsanitize=address")

add_executable(1
        main.cpp
        DbManager.cpp
        Request.cpp
        Stop.cpp
        Bus.cpp
        StringOp.cpp)