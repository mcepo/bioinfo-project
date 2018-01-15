cmake_minimum_required(VERSION 3.8)
project(bioinfo-project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES main.cpp FourRussians.cpp FourRussians.h ThreadPool.h generator.cpp)

#add_executable(generator generator.cpp)
add_executable(main main.cpp FourRussians.h FourRussians.cpp edlib/edlib.h edlib/edlib.cpp)
