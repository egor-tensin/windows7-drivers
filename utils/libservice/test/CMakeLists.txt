project(libservice_test)

project(libservice_test_windows_error)
add_executable(${PROJECT_NAME} windows_error.cpp)
target_link_libraries(${PROJECT_NAME} libservice)
