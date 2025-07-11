#include "Record.hpp"
#include <cstring>


void Record::serialize(std::vector<char>& buffer) const {
    buffer.resize(getRecordSize());

    char* ptr = buffer.data();

    memcpy(ptr, &id, sizeof(id));
    ptr += sizeof(id);

    memcpy(ptr, &name, sizeof(name));
    ptr += sizeof(name);

    memcpy(ptr, &age, sizeof(age));
    ptr += sizeof(age);

    memcpy(ptr, &balance, sizeof(balance));
}


void Record::deserialize(const std::vector<char>& buffer, size_t& offset) {
    const char* ptr = buffer.data() + offset;

    memcpy(&id, ptr, sizeof(id));
    ptr += sizeof(id);
    memcpy(&name, ptr, sizeof(name));
    ptr += sizeof(name);
    memcpy(&age, ptr, sizeof(age));
    ptr += sizeof(age);
    memcpy(&balance, ptr, sizeof(balance));
    offset += getRecordSize();
}