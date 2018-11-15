cmake_minimum_required(VERSION 3.1)

project(exec)

include(${CMAKE_SOURCE_DIR}/cmake/Common.cmake)

add_executable(main main.cpp)
target_link_libraries(main lib1)