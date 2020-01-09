# WIN32_LEAN_AND_MEAN shouldn't be defined, it masks CTL_CODE:
set(CC_WINDOWS_DEF OFF)

include(cmake/cmake/common.cmake)

add_subdirectory(service)
add_subdirectory(wrappers)
