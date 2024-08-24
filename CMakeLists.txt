cmake_minimum_required(VERSION 3.26)

project(
        lab3
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab3"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(bin)