#include <fstream>
#include <iostream>
#include <cstring>  // Añade este encabezado
#include <cerrno>   // Para errno

int main() {
    const char* filename = "C:\\Users\\johnf\\CLionProjects\\bonsaidb\\test.txt";

    std::ofstream file(filename);
    if (!file) {
        // Usa std::strerror con el namespace correcto
        std::cerr << "Error al crear archivo: " << std::strerror(errno) << std::endl;
        return 1;
    }
    
    file << "Prueba exitosa";
    file.close();
    std::cout << "Archivo creado: " << filename << std::endl;
    return 0;
}