add_executable(stress_test stress_test.cpp)
target_link_libraries(stress_test PRIVATE bonsaidb_lib)

add_executable(generar_datos generar_datos.cpp)