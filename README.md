# BonsaiDB

Motor de base de datos ligero escrito en C++17. Implementa un pequeño índice B+ y almacenamiento por páginas.

## Compilación
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Uso
Ejecutar la aplicación de línea de comandos:
```bash
./bonsaidb mydb.db
```

Para realizar pruebas de estrés:
```bash
./stress_test
```