cmake_minimum_required(VERSION 3.10)
project(2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -fsanitize=address")

add_executable(2
        main.cpp
        DbManager.cpp
        Request.cpp
        Stop.cpp
        Bus.cpp
        StringOp.cpp
        GeoFunction.cpp)
