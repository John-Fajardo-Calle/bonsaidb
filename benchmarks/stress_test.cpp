#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdio>
#include "DatabaseEngine.hpp"
#include <windows.h> // Para la codificación de la consola


int main() {
    SetConsoleOutputCP(CP_UTF8);
    const std::string db_file = "stress_test_final.db";
    std::remove(db_file.c_str());

    const int NUM_RECORDS = 50000;

    std::cout << "--- Iniciando Test de Estabilidad y Carga ---" << std::endl;
    size_t operations = 0;

    try {
        DatabaseEngine engine(db_file);

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
            operations++;

            if (operations % 5000 == 0) {
                std::cout << "  " << operations << " registros insertados..." << std::endl;
            }
        }
        std::cout << "Fase 1 completada. Todos los registros insertados." << std::endl;

        std::cout << "\nFase 2: Verificando " << NUM_RECORDS << " registros..." << std::endl;
        bool all_found = true;
        for (int i = 0; i < NUM_RECORDS; ++i) {
            auto found_record = engine.find(i);
            if (!found_record) {
                std::cerr << "Error Crítico: No se encontró el registro con ID " << i << std::endl;
                all_found = false;
            } else if (found_record->id != i) {
                std::cerr << "Error Crítico: Se encontró un registro incorrecto. Se esperaba ID " << i << ", pero se obtuvo " << found_record->id << std::endl;
                all_found = false;
            }
             if (i % 5000 == 0) {
                std::cout << "  " << i << " registros verificados..." << std::endl;
            }
        }

        if (all_found) {
            std::cout << "Fase 2 completada. Verificación exitosa." << std::endl;
        } else {
            std::cout << "Fase 2 falló. Se encontraron inconsistencias." << std::endl;
        }

    } catch (const std::bad_alloc& e) {
        std::cerr << "ERROR FATAL: Memoria agotada después de " << operations << " operaciones. what(): " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "ERROR FATAL: Ocurrió una excepción inesperada después de " << operations << " operaciones. what(): " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n--- Test de Estrés Finalizado con Éxito ---" << std::endl;
    return 0;
}