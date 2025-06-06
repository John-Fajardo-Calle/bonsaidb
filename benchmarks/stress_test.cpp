#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "DatabaseEngine.hpp"

int main() {
    const std::string db_file = "stress_test.db";
    // Borrar el archivo de la DB anterior si existe
    std::remove(db_file.c_str());

    DatabaseEngine engine(db_file);
    const int NUM_RECORDS = 1000;

    std::cout << "--- Iniciando Test de Estrés ---" << std::endl;
    std::cout << "Fase 1: Insertando " << NUM_RECORDS << " registros..." << std::endl;

    for (int i = 0; i < NUM_RECORDS; ++i) {
        Record r;
        r.id = i;
        snprintf(r.name, sizeof(r.name), "User%d", i);
        r.age = 20 + (i % 50);
        r.balance = 100.0 * i;

        if (!engine.insert(r)) {
            std::cerr << "Error: Falla al insertar el registro con ID " << i << std::endl;
            return 1;
        }
    }
    std::cout << "Fase 1 completada con éxito." << std::endl;

    std::cout << "\nFase 2: Verificando la existencia de todos los registros..." << std::endl;
    bool all_found = true;
    for (int i = 0; i < NUM_RECORDS; ++i) {
        auto found_record = engine.find(i);
        if (!found_record) {
            std::cerr << "Error: No se encontró el registro con ID " << i << ", pero debería existir." << std::endl;
            all_found = false;
        } else if (found_record->id != i) {
            std::cerr << "Error: Se encontró un registro incorrecto. Se esperaba ID " << i << ", pero se obtuvo " << found_record->id << std::endl;
            all_found = false;
        }
    }

    if (all_found) {
        std::cout << "Fase 2 completada con éxito. Todos los registros fueron encontrados correctamente." << std::endl;
    } else {
        std::cout << "Fase 2 falló. Algunos registros no se encontraron." << std::endl;
    }

    std::cout << "\n--- Test de Estrés Finalizado ---" << std::endl;

    return 0;
}