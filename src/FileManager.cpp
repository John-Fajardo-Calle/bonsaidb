#include "FileManager.hpp"
#include <iostream>


FileManager::FileManager(const std::string& db_filename) : filename(db_filename) {

    file_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file_stream.is_open()) {
        file_stream.open(filename, std::ios::out | std::ios::binary);
        file_stream.close();
        file_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }

    if (!file_stream.is_open()) {
        std::cerr << "Error: No se pudo abrir o crear el archivo de la base de datos: " << filename << std::endl;
    }
}

FileManager::~FileManager() {
    if (file_stream.is_open()) {
        file_stream.close();
    }
}

bool FileManager::writeRawPage(uint32_t pageId, const std::vector<char>& buffer) {
    if (!file_stream.is_open() || buffer.size() != PAGE_SIZE) return false;

    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;
    file_stream.seekp(offset, std::ios::beg);
    file_stream.write(buffer.data(), PAGE_SIZE);
    file_stream.flush();
    return file_stream.good();
}

bool FileManager::readRawPage(uint32_t pageId, std::vector<char>& buffer) {
    if (!file_stream.is_open()) return false;

    buffer.resize(PAGE_SIZE);
    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;
    file_stream.seekg(offset, std::ios::beg);
    file_stream.read(buffer.data(), PAGE_SIZE);

    return file_stream.gcount() > 0;
}

bool FileManager::writePage(uint32_t pageId, const Page& page) {
    if (!file_stream.is_open()) return false;

    std::vector<char> buffer;
    page.serialize(buffer); // Usamos la serialización propia de la clase Page

    // Asegurarnos que el buffer tenga el tamaño de página correcto
    if (buffer.size() != PAGE_SIZE) {
        buffer.resize(PAGE_SIZE, 0);
    }

    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;
    file_stream.seekp(offset, std::ios::beg);
    file_stream.write(buffer.data(), PAGE_SIZE);
    file_stream.flush();
    return file_stream.good();
}

bool FileManager::readPage(uint32_t pageId, Page& page) {
    if (!file_stream.is_open()) return false;

    std::vector<char> buffer(PAGE_SIZE);
    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;

    file_stream.seekg(offset, std::ios::beg);
    file_stream.read(buffer.data(), PAGE_SIZE);

    if (file_stream.gcount() > 0) {
        page.deserialize(buffer); // Usamos la deserialización de la clase Page
        return true;
    }

    return false;
}


uint32_t FileManager::allocatePage() {
    if (!file_stream.is_open()) return 0;

    file_stream.seekg(0, std::ios::end);
    size_t fileSize = file_stream.tellg();
    uint32_t new_page_id = static_cast<uint32_t>(fileSize / PAGE_SIZE);

    std::vector<char> empty(PAGE_SIZE, 0);
    file_stream.seekp(static_cast<size_t>(new_page_id) * PAGE_SIZE, std::ios::beg);
    file_stream.write(empty.data(), PAGE_SIZE);
    file_stream.flush();

    return new_page_id;
}

uint32_t FileManager::getNumPages() {
    if (!file_stream.is_open()) return 0;

    std::streampos current_pos = file_stream.tellg();
    file_stream.seekg(0, std::ios::end);
    size_t fileSize = file_stream.tellg();
    file_stream.seekg(current_pos, std::ios::beg);

    return static_cast<uint32_t>(fileSize / PAGE_SIZE);
}

