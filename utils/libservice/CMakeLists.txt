project(libservice)
file(GLOB ${PROJECT_NAME}_sources "src/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_headers "include/*.hpp")
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_sources}
                            ${${PROJECT_NAME}_headers})
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(test)
add_subdirectory(utils)
