#ifndef BONSAIDB_DATABASEENGINE_HPP
#define BONSAIDB_DATABASEENGINE_HPP

#include "FileManager.hpp"
#include "Record.hpp"
#include "BPlusTree.hpp"
#include <string>
#include <memory>  // Para std::unique_ptr
#include <optional> // Para std::optional
#include <mutex>    // Para std::mutex (preparando para la concurrencia)

/**
 * @class DatabaseEngine
 * @brief El motor principal de la base de datos que gestiona todas las operaciones.
 *
 * Esta clase es la interfaz pública para interactuar con la base de datos.
 * Utiliza un FileManager para la persistencia en disco y en el futuro
 * gestionará índices y concurrencia.
 */
class DatabaseEngine {
public:
    explicit DatabaseEngine(const std::string& db_path);
    ~DatabaseEngine();

    DatabaseEngine(const DatabaseEngine&) = delete;
    DatabaseEngine& operator=(const DatabaseEngine&) = delete;

    bool insert(const Record& record);
    std::optional<Record> find(int32_t id);
    bool remove(int32_t id);

private:
    std::unique_ptr<FileManager> file_manager;

    // Descomentar y cambiar a un puntero inteligente
    std::unique_ptr<BPlusTree> index;
    uint32_t next_data_page_id{1};
    std::mutex engine_mutex;
};

#endif //BONSAIDB_DATABASEENGINE_HPP