cmake_minimum_required(VERSION 3.21)
project(laba4)

set(CMAKE_CXX_STANDARD 98)


add_executable(Receiver constants.h receiver.cpp processMessage.h errors.h)
add_executable(Sender constants.h sender.cpp processMessage.h errors.h)