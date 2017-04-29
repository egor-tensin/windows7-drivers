file(GLOB_RECURSE nt_namespace_include "include/*.hpp")
file(GLOB nt_namespace_src "src/*.cpp")
add_library(nt_namespace ${nt_namespace_src} ${nt_namespace_include})
target_include_directories(nt_namespace PUBLIC include/)
target_link_libraries(nt_namespace PRIVATE service)

add_subdirectory(utils)
