cmake_minimum_required(VERSION 3.1)

project(win7drv CXX)

include(cmake/common.cmake)

add_subdirectory(service)
add_subdirectory(wrappers)
