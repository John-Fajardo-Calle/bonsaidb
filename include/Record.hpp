#ifndef BONSAIDB_RECORD_HPP
#define BONSAIDB_RECORD_HPP

#include <string>
#include <vector>
#include <cstdint>

struct Record {
    int32_t id;
    char name[50];
    int32_t age;
    double balance;
    void serialize(std::vector<char>& buffer) const;
    void deserialize(const std::vector<char>& buffer, size_t& offset);
    bool operator==(const Record& other) const {
        return id == other.id && std::string(name) == std::string(other.name) &&
               age == other.age && balance == other.balance;
    }
    static constexpr size_t getRecordSize() {
        return sizeof(id) + sizeof(name) + sizeof(age) + sizeof(balance);
    }
};

#endif