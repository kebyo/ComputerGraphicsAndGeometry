cmake_minimum_required(VERSION 3.12)
project(graph)

set(CMAKE_CXX_STANDARD 17)

add_executable(graph main.cpp CExpension.cpp CExpension.h CLine.cpp CLine.h CImage.cpp CImage.h)