cmake_minimum_required(VERSION 2.8)

project(Connections)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Wextra -Ofast" )

add_subdirectory(tests)
add_subdirectory(source)
