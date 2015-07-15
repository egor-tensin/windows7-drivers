file(GLOB_RECURSE libtest_headers "include/*.hpp")
file(GLOB libtest_sources "src/*.cpp")
add_library(libtest
    ${libtest_sources}
    ${libtest_headers})
target_link_libraries(libtest libservice)
target_include_directories(libtest PUBLIC include/)

add_subdirectory(utils)
