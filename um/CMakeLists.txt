cmake_minimum_required(VERSION 3.1)

project(win7drv CXX)

# WIN32_LEAN_AND_MEAN shouldn't be defined, it masks CTL_CODE:
set(CC_WINDOWS_DEF OFF)

include(cmake/common.cmake)

add_subdirectory(service)
add_subdirectory(wrappers)
