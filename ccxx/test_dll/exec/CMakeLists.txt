cmake_minimum_required(VERSION 3.1)

project(exec)

include(${CMAKE_SOURCE_DIR}/cmake/Common.cmake)

add_executable(main main.cpp)
target_link_libraries(main lib1)

message(STATUS "CMAKE_CXX_TARGET_INCLUDE_PATH：'${CMAKE_CXX_TARGET_INCLUDE_PATH}'")

message(STATUS "DEBUG：'${DEBUG}'")

# 用于测试
if(DEBUG)
    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(dir ${dirs})
        message(STATUS "dir='${dir}'")
    endforeach()
endif()