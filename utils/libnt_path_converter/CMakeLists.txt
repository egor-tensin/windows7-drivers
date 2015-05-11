project(libnt_path_converter)
file(GLOB ${PROJECT_NAME}_sources "src/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_headers "include/*.hpp")
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_sources}
                            ${${PROJECT_NAME}_headers})
target_link_libraries(${PROJECT_NAME} libservice)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

add_subdirectory(utils)
