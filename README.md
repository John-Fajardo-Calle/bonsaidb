# BonsaiDB
![build](https://img.shields.io/badge/build-passing-brightgreen)

Motor de base de datos ligero escrito en C++17. Implementa un pequeño índice B+ y almacenamiento por páginas.

## Compilación
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Objetivos del proyecto
BonsaiDB busca ofrecer un motor de base de datos sencillo que sirva para experimentar con estructuras de datos y técnicas de almacenamiento eficiente.

## Estructura del proyecto
- `src/` código fuente principal
- `include/` archivos de cabecera
- `benchmarks/` scripts y programas de benchmark
- `test/` pruebas unitarias


## Uso
Ejecutar la aplicación de línea de comandos:
```bash
./bonsaidb mydb.db
```

## Ejemplos de uso de la CLI
Dentro de la aplicación interactiva se pueden ejecutar comandos como:
```bash
insert 1 Juan 30 1000.0
select 1
dump
```

Para realizar pruebas de estrés:
```bash
./stress_test
```


## Benchmarks

Para compilar y ejecutar los benchmarks automáticamente:

```bash
./benchmarks/ejecutar_benchmarks.sh
```

El script generará un directorio temporal de compilación y colocará los
resultados en `benchmarks/resultados/bench.log`.

### Interpretación de los logs

El archivo `bench.log` muestra el avance por fases:

- **Fase 1** inserta registros y reporta el progreso cada 5000
  operaciones.
- **Fase 2** verifica los registros insertados de la misma forma.
- Cualquier línea que comience con `Error` o `ERROR FATAL` indica que la
  prueba falló.
- Si al final aparece `--- Test de Estrés Finalizado con Éxito ---`, el
  benchmark se completó correctamente.
## Contribuir
¡Se agradecen pull requests y reportes de errores!


## License
This project is licensed under the [MIT License](LICENSE).