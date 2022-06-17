cmake_minimum_required(VERSION 3.21)
project(laba4)

set(CMAKE_CXX_STANDARD 98)


add_executable(Receiver receiver.cpp processMessage.h)
add_executable(Sender sender.cpp processMessage.h)