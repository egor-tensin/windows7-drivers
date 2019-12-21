add_executable(simple_exchange_ints exchange_ints.cpp)
target_link_libraries(simple_exchange_ints PRIVATE simple)
set_target_properties(simple_exchange_ints PROPERTIES OUTPUT_NAME exchange_ints)

install(TARGETS simple_exchange_ints RUNTIME DESTINATION bin)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    install(FILES "$<TARGET_PDB_FILE:simple_exchange_ints>" DESTINATION bin OPTIONAL)
endif()
