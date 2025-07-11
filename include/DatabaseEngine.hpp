#ifndef BONSAIDB_DATABASEENGINE_HPP
#define BONSAIDB_DATABASEENGINE_HPP

#include "FileManager.hpp"
#include "Record.hpp"
#include "BPlusTree.hpp"
#include <string>
#include <memory>
#include <optional>
#include <mutex>
#include <shared_mutex>

class DatabaseEngine {
public:
    explicit DatabaseEngine(const std::string& db_path);
    ~DatabaseEngine();

    DatabaseEngine(const DatabaseEngine&) = delete;
    DatabaseEngine& operator=(const DatabaseEngine&) = delete;

    bool insert(const Record& record);
    std::optional<Record> find(int32_t id);
    bool remove(int32_t id);
    std::vector<Record> dumpAll();

private:
    std::unique_ptr<FileManager> file_manager;
    std::unique_ptr<BPlusTree> index;
    uint32_t next_data_page_id{0};
    std::shared_mutex engine_mutex;
};

#endif