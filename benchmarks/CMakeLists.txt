# Targets para benchmarks
add_executable(stress_test stress_test.cpp)
add_executable(generar_datos generar_datos.cpp)

# Vincular con la librería principal
target_link_libraries(stress_test PRIVATE bonsaidb_lib)
target_link_libraries(generar_datos PRIVATE bonsaidb_lib)