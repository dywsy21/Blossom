cmake_minimum_required(VERSION 3.10)

project(graph_max_matching)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(graph_max_matching 
    src/main.cpp
    src/graph_max_matching.cpp
    src/getopt.cpp
    src/graph.cpp
    src/unionfind.cpp
)

# set O3 optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
