file(GLOB_RECURSE simple_include "include/*.hpp")
file(GLOB simple_src "src/*.cpp")
add_library(simple ${simple_src} ${simple_include})
target_include_directories(simple PUBLIC include/)
target_link_libraries(simple PRIVATE service)

add_subdirectory(utils)
