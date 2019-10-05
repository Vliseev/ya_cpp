cmake_minimum_required(VERSION 3.12)
project(riestrukturizatsiia_koda)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

file(GLOB SRC
        "*.h"
        "*.cpp"
        )

add_executable(riestrukturizatsiia_koda ${SRC})