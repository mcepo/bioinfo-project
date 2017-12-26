cmake_minimum_required(VERSION 3.8)
project(bioinfV2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES main.cpp TableBlock.h TableBlock.cpp edlib.h edlib.cpp)
add_executable(bioinfV2 ${SOURCE_FILES})