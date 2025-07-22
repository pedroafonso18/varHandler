cmake_minimum_required(VERSION 3.31)
project(varHandler)

set(CMAKE_CXX_STANDARD 20)

add_executable(varHandler src/main.cpp
        include/database.h
        src/Database.cpp
)
