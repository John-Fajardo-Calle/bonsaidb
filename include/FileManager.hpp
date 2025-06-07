#ifndef BONSAIDB_FILEMANAGER_HPP
#define BONSAIDB_FILEMANAGER_HPP

#include "Page.hpp"
#include <string>
#include <fstream>


class FileManager {
public:
    explicit FileManager(const std::string& db_filename);
    ~FileManager();
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
    bool writePage(uint32_t pageId, const Page& page);
    bool readPage(uint32_t pageId, Page& page);
    uint32_t allocatePage();
    uint32_t getNumPages();
    bool writeRawPage(uint32_t pageId, const std::vector<char>& buffer);
    bool readRawPage(uint32_t pageId, std::vector<char>& buffer);

private:
    std::string filename;
    std::fstream file_stream;
};

#endif //BONSAIDB_FILEMANAGER_HPP