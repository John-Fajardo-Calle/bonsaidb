#include "Page.hpp"
#include <cstring>

Page::Page() : pageId(0), numRecords(0) {
    data.resize(PAGE_SIZE, 0);
}

size_t Page::getFreeSpace() const {
    const size_t metadataSize = sizeof(pageId) + sizeof(numRecords);
    const size_t recordsSize = numRecords * Record::getRecordSize();
    return PAGE_SIZE - metadataSize - recordsSize;
}

bool Page::addRecord(const Record& record) {
    if (getFreeSpace() < Record::getRecordSize()) {
        return false;
    }

    std::vector<char> recordBuffer;
    record.serialize(recordBuffer);

    const size_t offset = sizeof(pageId) + sizeof(numRecords) + (numRecords * Record::getRecordSize());

    memcpy(data.data() + offset, recordBuffer.data(), Record::getRecordSize());

    numRecords++;

    return true;
}


std::vector<Record> Page::getRecords() const {
    std::vector<Record> records;
    if (numRecords == 0) {
        return records;
    }

    records.reserve(numRecords);
    size_t offset = sizeof(pageId) + sizeof(numRecords);

    for (uint16_t i = 0; i < numRecords; ++i) {
        Record rec;
        rec.deserialize(this->data, offset);
        records.push_back(rec);
    }

    return records;
}
bool Page::removeRecord(int32_t id) {
    const size_t metadataSize = sizeof(pageId) + sizeof(numRecords);
    const size_t recordSize = Record::getRecordSize();

    for (uint16_t i = 0; i < numRecords; ++i) {
        size_t offset = metadataSize + i * recordSize;
        int32_t current_id;
        memcpy(&current_id, data.data() + offset, sizeof(current_id));
        if (current_id == id) {
            size_t bytes_after = (numRecords - i - 1) * recordSize;
            if (bytes_after > 0) {
                memmove(data.data() + offset,
                        data.data() + offset + recordSize,
                        bytes_after);
            }
            memset(data.data() + metadataSize + (numRecords - 1) * recordSize,
                   0,
                   recordSize);
            numRecords--;
            return true;
        }
    }
    return false;
}



void Page::serialize(std::vector<char>& buffer) const {
    buffer.resize(PAGE_SIZE);

    char* ptr = buffer.data();

    memcpy(ptr, &this->pageId, sizeof(this->pageId));
    ptr += sizeof(this->pageId);

    memcpy(ptr, &this->numRecords, sizeof(this->numRecords));
    ptr += sizeof(this->numRecords);
    const size_t recordsDataSize = this->numRecords * Record::getRecordSize();
    if (recordsDataSize > 0) {
        memcpy(ptr, this->data.data() + sizeof(pageId) + sizeof(numRecords), recordsDataSize);
    }
}


void Page::deserialize(const std::vector<char>& buffer) {
    if (buffer.size() != PAGE_SIZE) {
        return;
    }

    this->data = buffer;
    const char* ptr = this->data.data();
    memcpy(&this->pageId, ptr, sizeof(this->pageId));
    ptr += sizeof(this->pageId);

    memcpy(&this->numRecords, ptr, sizeof(this->numRecords));
}