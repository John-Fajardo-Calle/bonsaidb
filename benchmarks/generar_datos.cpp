#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <filesystem>
#include <limits>
#include <windows.h>


int main() {
    std::cout << "Ingrese la ruta o carpeta donde desea crear el CSV: ";
    std::string input_path;
    std::getline(std::cin, input_path);
    std::filesystem::path file_path = std::filesystem::absolute(input_path);
    if (!file_path.has_extension() || std::filesystem::is_directory(file_path)) {
        file_path /= "datos.csv";
    }
    std::cout << "Ingrese el número de registros a crear: ";

    int num_records = 0;

    while (!(std::cin >> num_records) || num_records <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Número inválido. Intente nuevamente: ";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    std::error_code ec;
    std::filesystem::create_directories(file_path.parent_path(), ec);
    if (ec) {
        std::cerr << "No se pudo crear el directorio del archivo: " << ec.message() << std::endl;
        return 1;
    }

    std::ofstream file(file_path, std::ios::trunc);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo para escritura: " << file_path << std::endl;
        return 1;
    }

    file << "id,nombre,edad,saldo\n";

    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> age_dist(18, 65);
    std::uniform_real_distribution<double> balance_dist(0.0, 10000.0);

    for (int i = 0; i < num_records; ++i) {
        file << i << "," << "User" << i << "," << age_dist(rng) << "," << balance_dist(rng) << "\n";
    }

    file.flush();
    if (!file) {
        std::cerr << "Error al escribir en el archivo: " << file_path << std::endl;
        return 1;
    }
    file.close();
    if (file.fail()) {
        std::cerr << "Error al cerrar el archivo: " << file_path << std::endl;
        return 1;
    }

    std::cout << "Archivo generado: " << file_path << " con " << num_records << " registros." << std::endl;
    return 0;
}