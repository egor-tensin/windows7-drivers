add_executable(nt_namespace_convert convert_nt_path.cpp)
target_link_libraries(nt_namespace_convert PRIVATE nt_namespace)
set_target_properties(nt_namespace_convert PROPERTIES OUTPUT_NAME convert_nt_path)
if(MINGW)
    target_compile_options(nt_namespace_convert PRIVATE -municode)
    target_link_libraries(nt_namespace_convert PRIVATE -municode)
endif()

install(TARGETS nt_namespace_convert RUNTIME DESTINATION bin)
