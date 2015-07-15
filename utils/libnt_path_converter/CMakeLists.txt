project(libnt_path_converter)
file(GLOB libnt_path_converter_sources "src/*.cpp")
file(GLOB_RECURSE libnt_path_converter_headers "include/*.hpp")
add_library(libnt_path_converter
    ${libnt_path_converter_sources}
    ${libnt_path_converter_headers})
target_link_libraries(libnt_path_converter libservice)
target_include_directories(libnt_path_converter PUBLIC include/)

add_subdirectory(utils)
