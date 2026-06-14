if(DEFINED ENV{APPVEYOR})
    # Required because the image used (Visual Studio 2013) ships with a CMake
    # version that really is too old.
    cmake_minimum_required(VERSION 3.12)
else()
    # oldstable as of May '26.
    cmake_minimum_required(VERSION 3.25)
endif()

project(win7drv CXX)

if(MSVC)
    add_compile_options(/MP /W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_compile_definitions(NOMINMAX)

add_subdirectory(um)

install(FILES LICENSE.txt DESTINATION share)
