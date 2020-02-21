cmake_minimum_required(VERSION 2.8)

project(cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -pthread -std=c++1y -Wall -Wextra")
#set(CPP_SOURCE Timer.cpp)
#set(HEADER Timer.h)
#add_library(timer ${CPP_SOURCE})

add_executable(cpp Workers.cpp)
