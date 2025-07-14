#include "DatabaseEngine.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <numeric>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif


long long measure_insertion_time(DatabaseEngine& engine, int num_records) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_records; ++i) {
        Record r;
        r.id = i;
        snprintf(r.name, sizeof(r.name), "User%d", i);
        r.age = 20 + (i % 50);
        r.balance = 100.0 * i;
        engine.insert(r);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

long long measure_search_time(DatabaseEngine& engine, int num_records_to_search) {
    std::vector<long long> timings;
    for (int i = 0; i < num_records_to_search; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        engine.find(i);
        auto end = std::chrono::high_resolution_clock::now();
        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }
    return std::accumulate(timings.begin(), timings.end(), 0LL) / timings.size();
}


int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    std::vector<int> record_counts = {1000, 10000, 50000, 100000};
    std::ofstream results_file("benchmarks/resultados/performance_results.csv");

    if (!results_file) {
        std::cerr << "Error: No se pudo abrir el archivo de resultados." << std::endl;
        std::filesystem::create_directory("benchmarks/resultados");
        results_file.open("benchmarks/resultados/performance_results.csv");
    }

    results_file << "num_records,insert_time_ms,search_time_ns\n";
    std::cout << "--- Iniciando Benchmarks de Rendimiento ---" << std::endl;
    std::cout << "Resultados se guardarán en: benchmarks/resultados/performance_results.csv" << std::endl;

    for (int count : record_counts) {
        std::cout << "\nProcesando benchmark para " << count << " registros..." << std::endl;
        const std::string db_file = "benchmark_" + std::to_string(count) + ".db";
        std::remove(db_file.c_str());

        DatabaseEngine engine(db_file);

        long long insert_ms = measure_insertion_time(engine, count);
        std::cout << "  - Tiempo de inserción: " << insert_ms << " ms" << std::endl;

        long long search_ns = measure_search_time(engine, 1000);
        std::cout << "  - Tiempo promedio de búsqueda: " << search_ns << " ns" << std::endl;

        results_file << count << "," << insert_ms << "," << search_ns << "\n";

        std::remove(db_file.c_str());
    }

    results_file.close();
    std::cout << "\n--- Benchmarks finalizados con éxito ---" << std::endl;
    return 0;
}