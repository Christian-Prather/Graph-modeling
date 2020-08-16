cmake_minimum_required(VERSION 3.11)
project(Maze)

set(CMAKE_CXX_STANDARD 14)

add_executable(Maze main.cpp)
target_link_libraries(Maze ${DEPENDENCIES})
