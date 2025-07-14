add_executable(performance_test performance_test.cpp)

target_link_libraries(performance_test PRIVATE bonsaidb_lib)

if(WIN32)
    add_executable(generar_datos generar_datos.cpp)
    target_link_libraries(generar_datos PRIVATE bonsaidb_lib)
endif()