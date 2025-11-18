cmake_minimum_required(VERSION 4.0)
project(hex_cpp)

set(CMAKE_CXX_STANDARD 23)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
ENDIF()

add_executable(hex_cpp main.cpp
        common.h
        chunk.h
        chunk.cpp
        value.h
        value.cpp
        vm.cpp
        vm.h
        compiler.h
        compiler.cpp
        scanner.cpp
        scanner.h
)
