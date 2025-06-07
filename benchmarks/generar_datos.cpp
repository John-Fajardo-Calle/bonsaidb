#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <filesystem>
#include <limits>
#include <windows.h>
#include <ShlObj.h> // NUEVO: Librería para carpetas especiales de Windows.

// NUEVO: Función para obtener la ruta del Escritorio del usuario.
// Devuelve una ruta de filesystem o una ruta vacía si falla.
std::filesystem::path obtener_ruta_escritorio() {
    PWSTR pszPath = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &pszPath);

    if (SUCCEEDED(hr)) {
        std::filesystem::path desktop_path(pszPath);
        CoTaskMemFree(pszPath); // Importante: Liberar la memoria que Windows asignó.
        return desktop_path;
    }

    return {}; // Devuelve una ruta vacía si hubo un error.
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // NUEVO: Obtener la ruta del escritorio para usarla como opción predeterminada.
    std::filesystem::path ruta_escritorio_defecto = obtener_ruta_escritorio();
    std::filesystem::path directory;

    // MODIFICADO: Paso 1, ahora con opción predeterminada.
    std::cout << "--- Paso 1: Seleccionar Directorio ---" << std::endl;
    if (!ruta_escritorio_defecto.empty()) {
        std::cout << "Presione Enter para usar el Escritorio o ingrese una ruta de directorio: ";
    } else {
        std::cout << "Ingrese el directorio donde desea guardar el CSV: ";
    }

    std::string dir_path_str;
    std::getline(std::cin, dir_path_str);

    if (dir_path_str.empty() && !ruta_escritorio_defecto.empty()) {
        directory = ruta_escritorio_defecto; // El usuario eligió la opción predeterminada.
    } else {
        directory = std::filesystem::absolute(dir_path_str); // El usuario ingresó una ruta.
    }

    // Paso 2: Pedir nombre del archivo (sin extensión) - Sin cambios
    std::cout << "\n--- Paso 2: Nombrar el Archivo ---" << std::endl;
    std::cout << "Ingrese el nombre del archivo CSV (sin extensión, dejar vacío para 'datos'): ";
    std::string filename;
    std::getline(std::cin, filename);

    // Limpiar y verificar el nombre del archivo
    filename.erase(filename.find_last_not_of(" \n\r\t") + 1);
    filename.erase(0, filename.find_first_not_of(" \n\r\t"));

    if (filename.empty()) {
        filename = "datos";
    }

    // Construir ruta completa
    std::filesystem::path file_path = directory / (filename + ".csv");

    // Paso 3: Pedir número de registros - Sin cambios
    std::cout << "\n--- Paso 3: Definir Registros ---" << std::endl;
    std::cout << "Ingrese el número de registros a crear: ";
    int num_records = 0;

    while (!(std::cin >> num_records) || num_records <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Número inválido. Intente nuevamente: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Crear directorios si no existen
    std::error_code ec;
    std::filesystem::create_directories(file_path.parent_path(), ec);
    if (ec) {
        std::cerr << "No se pudo crear el directorio: " << ec.message() << std::endl;
        return 1;
    }

    // Crear y escribir el archivo CSV
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