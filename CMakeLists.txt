cmake_minimum_required(VERSION 3.22)
project(tinyshell)

set(CMAKE_CXX_STANDARD 23)

add_executable(tinyshell src/main.cpp
        src/util.cpp)
