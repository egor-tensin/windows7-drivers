add_executable(service_test_windows_error windows_error.cpp)
if(MSVC)
    # "unreachable code", what? Shut up.
    target_compile_options(service_test_windows_error PRIVATE /wd4702)
endif()
target_link_libraries(service_test_windows_error PRIVATE service)
set_target_properties(service_test_windows_error PROPERTIES OUTPUT_NAME windows_error)
