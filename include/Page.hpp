#ifndef BONSAIDB_PAGE_HPP
#define BONSAIDB_PAGE_HPP

#include "Record.hpp"
#include <vector>
#include <cstdint>

constexpr size_t PAGE_SIZE = 4096;

class Page {
public:
    Page();
    bool addRecord(const Record& record);
    std::vector<Record> getRecords() const;
    bool removeRecord(int32_t id);
    void serialize(std::vector<char>& buffer) const;
    void deserialize(const std::vector<char>& buffer);
    uint16_t getNumRecords() const { return numRecords; }
    size_t getFreeSpace() const;

private:
    uint32_t pageId;
    uint16_t numRecords;
    std::vector<char> data; 
};

#endif