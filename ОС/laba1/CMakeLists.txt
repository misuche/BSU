cmake_minimum_required(VERSION 3.21)
project(laba1)

set(CMAKE_CXX_STANDARD 98)

add_executable(main main.cpp employee.h)
add_executable(creator creator.cpp employee.h)
add_executable(reporter reporter.cpp employee.h)
