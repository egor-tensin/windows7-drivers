file(GLOB libservice_sources "src/*.cpp")
file(GLOB_RECURSE libservice_headers "include/*.hpp")
add_library(libservice
    ${libservice_sources}
    ${libservice_headers})
target_include_directories(libservice PUBLIC include)
target_compile_definitions(libservice PRIVATE NOMINMAX)

add_subdirectory(test)
add_subdirectory(utils)
