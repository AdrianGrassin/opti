cmake_minimum_required(VERSION 3.22)
project(opti)

set(CMAKE_CXX_STANDARD 14)

add_executable(opti src/main.cpp include/grafo.h src/grafo.cpp)
