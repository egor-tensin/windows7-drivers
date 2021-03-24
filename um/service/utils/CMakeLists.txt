function(add_util action src)
    set(target "service_${action}")
    set(name "${action}_service")

    add_executable("${target}" ${src})
    target_link_libraries("${target}" PRIVATE service)
    set_target_properties("${target}" PROPERTIES OUTPUT_NAME "${name}")

    install(TARGETS "${target}" RUNTIME DESTINATION bin)
    install_pdbs(TARGETS "${target}" DESTINATION bin)
endfunction()

add_util(install install_service.cpp)
add_util(start start_service.cpp)
add_util(stop stop_service.cpp)
add_util(uninstall uninstall_service.cpp)
