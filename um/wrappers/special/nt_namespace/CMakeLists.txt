project(libnt_namespace)
file(GLOB libnt_namespace_sources "src/*.cpp")
file(GLOB_RECURSE libnt_namespace_headers "include/*.hpp")
add_library(libnt_namespace
    ${libnt_namespace_sources}
    ${libnt_namespace_headers})
target_link_libraries(libnt_namespace libservice)
target_include_directories(libnt_namespace PUBLIC include/)

add_subdirectory(utils)
