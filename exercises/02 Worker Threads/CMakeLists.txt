cmake_minimum_required(VERSION 2.8)

project(cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++1y -Wall -Wextra")

add_executable(cpp Workers.cpp)
