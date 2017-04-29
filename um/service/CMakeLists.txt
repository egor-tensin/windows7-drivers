file(GLOB_RECURSE service_include "include/*.hpp")
file(GLOB service_src "src/*.cpp")
add_library(service ${service_src} ${service_include})
target_compile_definitions(service PRIVATE NOMINMAX)
target_include_directories(service PUBLIC include/)

add_subdirectory(test)
add_subdirectory(utils)
