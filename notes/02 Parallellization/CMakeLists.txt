cmake_minimum_required(VERSION 2.8)

project(atomic)
# -fsanitize=thread to aggressively check
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -pthread -std=c++1y -Wall -Wextra")

add_executable(atomic functional.cpp)
