cmake_minimum_required(VERSION 3.1)

project(lib1)

include(${CMAKE_SOURCE_DIR}/cmake/Common.cmake)

add_library(lib1 SHARED HelloLib.cpp)
