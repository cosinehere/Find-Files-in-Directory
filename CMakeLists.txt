cmake_minimum_required(VERSION 3.10)

project(FindFilesInDirectory
    VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(test
    ${CMAKE_SOURCE_DIR}/test.cpp
    )

