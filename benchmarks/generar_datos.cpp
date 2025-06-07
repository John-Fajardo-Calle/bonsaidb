#include <fstream>
#include <iostream>
#include <random>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <archivo.csv> <num_registros>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    int num_records = 0;
    try {
        num_records = std::stoi(argv[2]);
    } catch (...) {
        std::cerr << "Número de registros inválido" << std::endl;
        return 1;
    }

    std::ofstream file(filename, std::ios::trunc);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo para escritura: " << filename << std::endl;
        return 1;
    }

    file << "id,nombre,edad,saldo\n";

    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> age_dist(18, 65);
    std::uniform_real_distribution<double> balance_dist(0.0, 10000.0);

    for (int i = 0; i < num_records; ++i) {
        file << i << "," << "User" << i << "," << age_dist(rng) << "," << balance_dist(rng) << "\n";
    }

    std::cout << "Archivo generado: " << filename << " con " << num_records << " registros." << std::endl;
    return 0;
}