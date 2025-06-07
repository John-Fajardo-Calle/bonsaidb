#include "DatabaseEngine.hpp"
#include <iostream>
#include <vector>

constexpr uint32_t INDEX_ROOT_PAGE_ID = 0;

DatabaseEngine::DatabaseEngine(const std::string& db_path) {
    file_manager = std::make_unique<FileManager>(db_path);
    index = std::make_unique<BPlusTree>(*file_manager);
    next_data_page_id = file_manager->getNumPages();
}

DatabaseEngine::~DatabaseEngine() = default;

bool DatabaseEngine::insert(const Record& record) {
    std::lock_guard<std::mutex> lock(engine_mutex);
    uint32_t data_page_to_use = next_data_page_id;
    uint32_t num_pages = file_manager->getNumPages();

    Page page;
    if (data_page_to_use < num_pages) {
        file_manager->readPage(data_page_to_use, page);
    }

    if (!page.addRecord(record)) {
        data_page_to_use = file_manager->allocatePage();
        Page new_page;
        new_page.addRecord(record);
        page = new_page;
        next_data_page_id = data_page_to_use;
    }

    if (!file_manager->writePage(data_page_to_use, page)) {
        return false;
    }

    index->insert(record.id, data_page_to_use);
    std::cout << "Registro de datos guardado en página " << data_page_to_use << std::endl;
    return true;
}

std::optional<Record> DatabaseEngine::find(int32_t id) {
    std::lock_guard<std::mutex> lock(engine_mutex);
    std::optional<uint32_t> page_id_opt = index->search(id);

    if (!page_id_opt) {
        return std::nullopt;
    }

    uint32_t data_page_id = *page_id_opt;
    Page page;
    if (!file_manager->readPage(data_page_id, page)) {
        return std::nullopt;
    }

    std::vector<Record> records = page.getRecords();
    for (const auto& record : records) {
        if (record.id == id) {
            return record;
        }
    }
    return std::nullopt;
}

bool DatabaseEngine::remove(int32_t id) {
    std::lock_guard<std::mutex> lock(engine_mutex);
    if (index->remove(id)) {
        std::cout << "Registro con ID " << id << " eliminado del índice (funcionalidad parcial)." << std::endl;
        return true;
    } else {
        std::cout << "Error: No se encontró el registro con ID " << id << " en el índice." << std::endl;
        return false;
    }
}

std::vector<Record> DatabaseEngine::dumpAll() {
    std::lock_guard<std::mutex> lock(engine_mutex);
    std::vector<Record> all;
    auto page_ids = index->get_all_data_page_ids();
    for (uint32_t pid : page_ids) {
        Page p;
        if (file_manager->readPage(pid, p)) {
            auto recs = p.getRecords();
            all.insert(all.end(), recs.begin(), recs.end());
        }
    }
    return all;
}