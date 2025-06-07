#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#endif

#include "DatabaseEngine.hpp"
#include "Record.hpp"

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void print_help() {
    std::cout << "BonsaiDB - Comandos Disponibles:\n"
              << "  insert <id> <nombre> <edad> <saldo>  - Inserta un nuevo registro.\n"
              << "     <id>    : Entero de 32 bits (ej: 101)\n"
              << "     <nombre>: Texto sin espacios (max 49 carácteres, ej: Juan)\n"
              << "     <edad>  : Entero de 32 bits (ej: 30)\n"
              << "     <saldo> : Decimal (ej: 1500.75)\n"
              << "  select <id>                            - Busca un registro por su ID.\n"
              << "  delete <id>                            - (No funcional aún) Elimina un registro por ID.\n"
              << "  dump                                  - Muestra todos los registros.\n"
              << "  help                                   - Muestra esta ayuda.\n"
              << "  exit                                   - Cierra la aplicación.\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.db>" << std::endl;
        return 1;
    }

    std::string db_filename = argv[1];
    DatabaseEngine engine(db_filename);

    std::cout << "BonsaiDB iniciado. Usando archivo '" << db_filename << "'.\n"
              << "Escribe 'help' para ver la lista de comandos o 'exit' para salir." << std::endl;

    std::string line;
    while (true) {
        std::cout << "bonsaidb> ";
        if (!std::getline(std::cin, line)) {
            break; // Fin de la entrada (Ctrl+D)
        }

        auto tokens = split(line, ' ');
        if (tokens.empty()) {
            continue;
        }

        const std::string& command = tokens[0];

        if (command == "exit" || command == "quit") {
            break;
        } else if (command == "help") {
            print_help();
        } else if (command == "insert") {
            if (tokens.size() != 5) {
                std::cerr << "Error: 'insert' requiere 4 argumentos. Ver 'help'." << std::endl;
                continue;
            }
            try {
                Record new_record{};
                new_record.id = std::stoi(tokens[1]);
                strncpy(new_record.name, tokens[2].c_str(), sizeof(new_record.name) - 1);
                new_record.name[sizeof(new_record.name) - 1] = '\0'; // Asegurar terminación nula.
                new_record.age = std::stoi(tokens[3]);
                new_record.balance = std::stod(tokens[4]);

                if (engine.insert(new_record)) {
                    std::cout << "Registro insertado con éxito." << std::endl;
                } else {
                    std::cerr << "Error: No se pudo insertar el registro." << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Argumento inválido. ID y edad deben ser enteros, saldo debe ser un número." << std::endl;
            }
        } else if (command == "select") {
            if (tokens.size() != 2) {
                std::cerr << "Error: 'select' requiere un ID. Ver 'help'." << std::endl;
                continue;
            }
            try {
                int32_t id = std::stoi(tokens[1]);
                auto result = engine.find(id);
                if (result) {
                    const Record& rec = *result;
                    std::cout << "Registro encontrado:\n"
                              << "  ID     : " << rec.id << "\n"
                              << "  Nombre : " << rec.name << "\n"
                              << "  Edad   : " << rec.age << "\n"
                              << "  Saldo  : " << rec.balance << std::endl;
                } else {
                    std::cout << "Registro con ID " << id << " no encontrado." << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: El ID debe ser un número entero." << std::endl;
            }
        } else if (command == "delete") {
             if (tokens.size() != 2) {
                std::cerr << "Error: 'delete' requiere un ID. Ver 'help'." << std::endl;
                continue;
            }
            engine.remove(std::stoi(tokens[1]));
        } else if (command == "dump") {
            auto all = engine.dumpAll();
            for (const auto& rec : all) {
                std::cout << rec.id << "," << rec.name << "," << rec.age << "," << rec.balance << "\n";
            }
        }
        else {
            std::cerr << "Comando desconocido: '" << command << "'. Escribe 'help' para ayuda." << std::endl;
        }
    }

    std::cout << "Cerrando BonsaiDB." << std::endl;
    return 0;
}