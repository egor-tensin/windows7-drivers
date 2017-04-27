file(GLOB_RECURSE libsimple_headers "include/*.hpp")
file(GLOB libsimple_sources "src/*.cpp")
add_library(libsimple
    ${libsimple_sources}
    ${libsimple_headers})
target_link_libraries(libsimple libservice)
target_include_directories(libsimple PUBLIC include/)

add_subdirectory(utils)
