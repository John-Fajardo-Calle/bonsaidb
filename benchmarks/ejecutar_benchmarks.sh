#!/bin/bash
set -e

echo "--- Iniciando Proceso Completo de Benchmarking ---"

# --- PASO 1: Compilar el código de C++ ---
echo "[1/3] Preparando el entorno de compilación..."
BUILD_DIR="build_bench"
rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# CORRECCIÓN 1: Especificar el generador "MinGW Makefiles" para cmake.
# CORRECCIÓN 2: Eliminar "> /dev/null" para poder ver los mensajes y errores.
echo "Ejecutando CMake..."
cmake -G "MinGW Makefiles" ..

# CORRECIÓN 3: Usar el nombre correcto (mingw32-make) y eliminar -j$(nproc) que no es estándar en Windows.
echo "Compilando el proyecto con mingw32-make..."
mingw32-make

# Regresar al directorio raíz
cd ..
echo "Compilación finalizada con éxito."

# --- PASO 2: Ejecutar el test de rendimiento ---
echo "[2/3] Ejecutando el test para generar los datos CSV..."
mkdir -p "benchmarks/resultados"
# La ruta del ejecutable en Windows puede variar, esta es más robusta.
./"$BUILD_DIR"/benchmarks/performance_test.exe

# --- PASO 3: Generar la gráfica ---
echo "[3/3] Generando la gráfica a partir de los resultados CSV..."
if [ ! -d "venv_bench" ]; then
    python -m venv venv_bench
fi
source venv_bench/Scripts/activate
pip install pandas matplotlib > /dev/null
cd benchmarks
python plot_results.py
deactivate
cd ..

echo ""
echo "--- PROCESO FINALIZADO CON ÉXITO ---"
echo "La gráfica se ha guardado en: benchmarks/resultados/grafica_rendimiento.png"