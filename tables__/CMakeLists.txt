cmake_minimum_required(VERSION 3.8)
project(BioInf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp TableBlock.h TableBlock.cpp)
add_executable(BioInf ${SOURCE_FILES})
