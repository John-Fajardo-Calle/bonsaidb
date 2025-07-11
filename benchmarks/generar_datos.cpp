#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <filesystem>
#include <limits>
#ifdef _WIN32
#  include <windows.h>
#  include <ShlObj.h>
#endif

std::filesystem::path obtener_ruta_escritorio() {
#ifdef _WIN32
    PWSTR pszPath = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &pszPath);

    if (SUCCEEDED(hr)) {
        std::filesystem::path desktop_path(pszPath);
        CoTaskMemFree(pszPath);
        return desktop_path;
    }

    return {};
#else
    return {};
#endif
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::filesystem::path ruta_escritorio_defecto = obtener_ruta_escritorio();
    std::filesystem::path directory;

    std::cout << "--- Paso 1: Seleccionar Directorio ---" << std::endl;
    if (!ruta_escritorio_defecto.empty()) {
        std::cout << "Presione Enter para usar el Escritorio o ingrese una ruta de directorio: ";
    } else {
        std::cout << "Ingrese el directorio donde desea guardar el CSV: ";
    }

    std::string dir_path_str;
    std::getline(std::cin, dir_path_str);

    if (dir_path_str.empty() && !ruta_escritorio_defecto.empty()) {
        directory = ruta_escritorio_defecto;
    } else {
        directory = std::filesystem::absolute(dir_path_str);
    }

    std::cout << "\n--- Paso 2: Nombrar el Archivo ---" << std::endl;
    std::cout << "Ingrese el nombre del archivo CSV (sin extensión, dejar vacío para 'datos'): ";
    std::string filename;
    std::getline(std::cin, filename);

    filename.erase(filename.find_last_not_of(" \n\r\t") + 1);
    filename.erase(0, filename.find_first_not_of(" \n\r\t"));

    if (filename.empty()) {
        filename = "datos";
    }

    std::filesystem::path file_path = directory / (filename + ".csv");

    std::cout << "\n--- Paso 3: Definir Registros ---" << std::endl;
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
        std::cerr << "No se pudo crear el directorio: " << ec.message() << std::endl;
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

    std::cout << "\n¡Archivo CSV generado exitosamente!\n";
    std::cout << "Ruta: " << file_path << "\n";
    std::cout << "Registros: " << num_records << std::endl;

    return 0;
}