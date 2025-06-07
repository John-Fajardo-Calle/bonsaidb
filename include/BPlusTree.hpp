// BPlusTree.hpp
#ifndef BONSAIDB_BPLUSTREE_HPP
#define BONSAIDB_BPLUSTREE_HPP

#include <vector>
#include <cstdint>
#include <optional>
#include "Page.hpp"
#include "FileManager.hpp"

struct BPlusNode {
    bool is_leaf = false;
    uint32_t self_page_id = 0;
    uint32_t parent_page_id = 0;
    std::vector<int32_t> keys;
    std::vector<uint32_t> children_page_ids;
    std::vector<uint32_t> data_page_ids;
    uint32_t next_leaf_id = 0;

    void serialize(std::vector<char>& buffer) const;
    void deserialize(const std::vector<char>& buffer);
};

constexpr int BPLUS_TREE_ORDER = 50;
constexpr int MIN_KEYS = (BPLUS_TREE_ORDER - 1) / 2;

class BPlusTree {
public:
    explicit BPlusTree(FileManager& file_manager);
    void insert(int32_t key, uint32_t data_page_id);
    std::optional<uint32_t> search(int32_t key);
    bool remove(int32_t key);

private:
    FileManager& file_manager;
    uint32_t root_page_id;

    BPlusNode read_node(uint32_t page_id);
    void write_node(uint32_t page_id, const BPlusNode& node);
    BPlusNode find_leaf_node(int32_t key, uint32_t& page_id);
    void insert_into_parent(BPlusNode& left_child, int32_t key, uint32_t right_child_page_id);
    void remove_entry(uint32_t node_id, int32_t key);
};

#endif // BONSAIDB_BPLUSTREE_HPP
