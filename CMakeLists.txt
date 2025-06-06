cmake_minimum_required(VERSION 3.20)
project(bonsaidb VERSION 1.0 LANGUAGES CXX)

# --- Configuración Estándar de C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-march=x86-64)

# --- PASO 1: Definir todos los archivos fuente de nuestro motor ---
# Se listan todos los .cpp que componen la lógica de la base de datos.
# No se incluyen los puntos de entrada como main.cpp o stress_test.cpp.
set(BONSAIDB_SOURCES
        src/Record.cpp
        src/Page.cpp
        src/FileManager.cpp
        src/BPlusTree.cpp
        src/DatabaseEngine.cpp
)

# --- PASO 2: Crear una biblioteca con el código del motor ---
# Esto compila el motor en una unidad reutilizable llamada "bonsaidb_lib".
add_library(bonsaidb_lib ${BONSAIDB_SOURCES})

# --- PASO 3: Indicar a la biblioteca (y a quienes la usen) dónde encontrar sus headers ---
target_include_directories(bonsaidb_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- PASO 4: Crear el ejecutable de la CLI ---
# Se crea el programa "bonsaidb" SÓLO a partir de su propio punto de entrada.
add_executable(bonsaidb src/main.cpp)
# Y se enlaza con la biblioteca del motor para que pueda usar todas sus clases.
target_link_libraries(bonsaidb PRIVATE bonsaidb_lib)

# --- PASO 5: Crear el nuevo ejecutable de prueba ---
# Se crea el programa "stress_test" a partir de su propio punto de entrada.
add_executable(stress_test benchmarks/stress_test.cpp)
# Y también se enlaza con la biblioteca del motor.
target_link_libraries(stress_test PRIVATE bonsaidb_lib)