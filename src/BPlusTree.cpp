#include "BPlusTree.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_set>

void BPlusNode::serialize(std::vector<char>& buffer) const {
    buffer.assign(PAGE_SIZE, 0);
    char* ptr = buffer.data();
    memcpy(ptr, &is_leaf, sizeof(is_leaf)); ptr += sizeof(is_leaf);
    memcpy(ptr, &self_page_id, sizeof(self_page_id)); ptr += sizeof(self_page_id);
    memcpy(ptr, &parent_page_id, sizeof(parent_page_id)); ptr += sizeof(parent_page_id);
    memcpy(ptr, &next_leaf_id, sizeof(next_leaf_id)); ptr += sizeof(next_leaf_id);
    size_t count = keys.size();
    memcpy(ptr, &count, sizeof(count)); ptr += sizeof(count);
    if (!keys.empty()) memcpy(ptr, keys.data(), count * sizeof(int32_t));
    ptr += keys.size() * sizeof(int32_t);
    count = children_page_ids.size();
    memcpy(ptr, &count, sizeof(count)); ptr += sizeof(count);
    if (!children_page_ids.empty()) memcpy(ptr, children_page_ids.data(), count * sizeof(uint32_t));
    ptr += children_page_ids.size() * sizeof(uint32_t);
    count = data_page_ids.size();
    memcpy(ptr, &count, sizeof(count)); ptr += sizeof(count);
    if (!data_page_ids.empty()) memcpy(ptr, data_page_ids.data(), count * sizeof(uint32_t));
}

void BPlusNode::deserialize(const std::vector<char>& buffer) {
    const char* ptr = buffer.data();
    memcpy(&is_leaf, ptr, sizeof(is_leaf)); ptr += sizeof(is_leaf);
    memcpy(&self_page_id, ptr, sizeof(self_page_id)); ptr += sizeof(self_page_id);
    memcpy(&parent_page_id, ptr, sizeof(parent_page_id)); ptr += sizeof(parent_page_id);
    memcpy(&next_leaf_id, ptr, sizeof(next_leaf_id)); ptr += sizeof(next_leaf_id);
    size_t count;
    memcpy(&count, ptr, sizeof(count)); ptr += sizeof(count);
    keys.resize(count);
    if (count > 0) memcpy(keys.data(), ptr, count * sizeof(int32_t));
    ptr += count * sizeof(int32_t);
    memcpy(&count, ptr, sizeof(count)); ptr += sizeof(count);
    children_page_ids.resize(count);
    if (count > 0) memcpy(children_page_ids.data(), ptr, count * sizeof(uint32_t));
    ptr += count * sizeof(uint32_t);
    memcpy(&count, ptr, sizeof(count)); ptr += sizeof(count);
    data_page_ids.resize(count);
    if (count > 0) memcpy(data_page_ids.data(), ptr, count * sizeof(uint32_t));
}

BPlusTree::BPlusTree(FileManager& fm) : file_manager(fm) {
    root_page_id = file_manager.readRootPageId();
    if (root_page_id == 0) {
        root_page_id = file_manager.allocatePage();
        BPlusNode root;
        root.self_page_id = root_page_id;
        root.is_leaf = true;
        write_node(root_page_id, root);
        file_manager.writeRootPageId(root_page_id);
    }
}

std::optional<uint32_t> BPlusTree::search(int32_t key) {
    uint32_t current_page_id = root_page_id;
    while (true) {
        BPlusNode node = read_node(current_page_id);
        if (node.is_leaf) {
            auto it = std::lower_bound(node.keys.begin(), node.keys.end(), key);
            if (it != node.keys.end() && *it == key) {
                return node.data_page_ids[std::distance(node.keys.begin(), it)];
            }
            return std::nullopt;
        }
        auto it = std::upper_bound(node.keys.begin(), node.keys.end(), key);
        current_page_id = node.children_page_ids[std::distance(node.keys.begin(), it)];
    }
}

BPlusNode BPlusTree::find_leaf_node(int32_t key, uint32_t& page_id) {
    page_id = root_page_id;
    while (true) {
        BPlusNode node = read_node(page_id);
        if (node.is_leaf) {
            return node;
        }
        auto it = std::upper_bound(node.keys.begin(), node.keys.end(), key);
        page_id = node.children_page_ids[std::distance(node.keys.begin(), it)];
    }
}

void BPlusTree::insert(int32_t key, uint32_t data_page_id) {
    uint32_t current_page_id = root_page_id;
    BPlusNode node = read_node(current_page_id);

    while (!node.is_leaf) {
        auto it = std::upper_bound(node.keys.begin(), node.keys.end(), key);
        current_page_id = node.children_page_ids[std::distance(node.keys.begin(), it)];
        node = read_node(current_page_id);
    }

    auto it = std::lower_bound(node.keys.begin(), node.keys.end(), key);
    if (it != node.keys.end() && *it == key) return;

    size_t pos = std::distance(node.keys.begin(), it);
    node.keys.insert(node.keys.begin() + pos, key);
    node.data_page_ids.insert(node.data_page_ids.begin() + pos, data_page_id);

    if (node.keys.size() < BPLUS_TREE_ORDER) {
        write_node(node.self_page_id, node);
    } else {
        BPlusNode new_sibling;
        new_sibling.is_leaf = true;
        new_sibling.self_page_id = file_manager.allocatePage();
        new_sibling.parent_page_id = node.parent_page_id;

        size_t const mid_point = node.keys.size() / 2;
        int32_t promoted_key = node.keys[mid_point];

        new_sibling.keys.assign(node.keys.begin() + mid_point, node.keys.end());
        new_sibling.data_page_ids.assign(node.data_page_ids.begin() + mid_point, node.data_page_ids.end());

        node.keys.resize(mid_point);
        node.data_page_ids.resize(mid_point);

        new_sibling.next_leaf_id = node.next_leaf_id;
        node.next_leaf_id = new_sibling.self_page_id;

        write_node(node.self_page_id, node);
        write_node(new_sibling.self_page_id, new_sibling);

        insert_into_parent(node, promoted_key, new_sibling.self_page_id);
    }
}

void BPlusTree::insert_into_parent(BPlusNode& left_child, int32_t key, uint32_t right_child_id) {
    if (left_child.parent_page_id == 0) {
        uint32_t new_root_id = file_manager.allocatePage();
        BPlusNode new_root;
        new_root.self_page_id = new_root_id;
        new_root.keys.push_back(key);
        new_root.children_page_ids.push_back(left_child.self_page_id);
        new_root.children_page_ids.push_back(right_child_id);

        root_page_id = new_root_id;
        left_child.parent_page_id = root_page_id;
        BPlusNode right_child = read_node(right_child_id);
        right_child.parent_page_id = root_page_id;

        write_node(root_page_id, new_root);
        write_node(left_child.self_page_id, left_child);
        write_node(right_child_id, right_child);
        file_manager.writeRootPageId(root_page_id);
        return;
    }

    BPlusNode parent = read_node(left_child.parent_page_id);
    auto it = std::upper_bound(parent.keys.begin(), parent.keys.end(), key);
    size_t pos = std::distance(parent.keys.begin(), it);
    parent.keys.insert(parent.keys.begin() + pos, key);
    parent.children_page_ids.insert(parent.children_page_ids.begin() + pos + 1, right_child_id);

    if (parent.keys.size() < BPLUS_TREE_ORDER) {
        write_node(parent.self_page_id, parent);
    } else {
        BPlusNode new_sibling;
        new_sibling.self_page_id = file_manager.allocatePage();
        new_sibling.parent_page_id = parent.parent_page_id;

        size_t mid_point = parent.keys.size() / 2;
        int32_t promoted_key = parent.keys[mid_point];

        new_sibling.keys.assign(parent.keys.begin() + mid_point + 1, parent.keys.end());
        new_sibling.children_page_ids.assign(parent.children_page_ids.begin() + mid_point + 1, parent.children_page_ids.end());

        parent.keys.resize(mid_point);
        parent.children_page_ids.resize(mid_point + 1);

        for (uint32_t child_id : new_sibling.children_page_ids) {
            BPlusNode child = read_node(child_id);
            child.parent_page_id = new_sibling.self_page_id;
            write_node(child_id, child);
        }

        write_node(parent.self_page_id, parent);
        write_node(new_sibling.self_page_id, new_sibling);

        insert_into_parent(parent, promoted_key, new_sibling.self_page_id);
    }
}

bool BPlusTree::remove(int32_t key) {
    uint32_t leaf_page_id;
    BPlusNode leaf = find_leaf_node(key, leaf_page_id);

    auto it = std::lower_bound(leaf.keys.begin(), leaf.keys.end(), key);

    if (it == leaf.keys.end() || *it != key) {
        return false;
    }

    size_t pos = std::distance(leaf.keys.begin(), it);
    leaf.keys.erase(it);
    leaf.data_page_ids.erase(leaf.data_page_ids.begin() + pos);

    write_node(leaf_page_id, leaf);

    return true;
}

void BPlusTree::remove_entry(uint32_t node_id, int32_t key) {
    BPlusNode node = read_node(node_id);
    auto it = std::lower_bound(node.keys.begin(), node.keys.end(), key);
    size_t pos = std::distance(node.keys.begin(), it);

    if (node.is_leaf) {
        if (it != node.keys.end() && *it == key) {
            node.keys.erase(it);
            node.data_page_ids.erase(node.data_page_ids.begin() + pos);
        }
    } else {
        uint32_t child_id = node.children_page_ids[pos];
        remove_entry(child_id, key);
    }

    node = read_node(node_id);
    if (node.self_page_id == root_page_id) {
        if (!node.is_leaf && node.keys.empty()) {
            root_page_id = node.children_page_ids[0];
            BPlusNode new_root = read_node(root_page_id);
            new_root.parent_page_id = 0;
            write_node(root_page_id, new_root);
        }
        return;
    }

    if (node.keys.size() >= MIN_KEYS) {
        write_node(node_id, node);
        return;
    }

    BPlusNode parent = read_node(node.parent_page_id);
    auto parent_it = std::upper_bound(parent.keys.begin(), parent.keys.end(), node.keys.front());
    size_t node_idx = std::distance(parent.keys.begin(), parent_it);

    if (node_idx > 0) {
        uint32_t left_sibling_id = parent.children_page_ids[node_idx - 1];
        BPlusNode left_sibling = read_node(left_sibling_id);
        if (left_sibling.keys.size() > MIN_KEYS) {
            if (node.is_leaf) {
                node.keys.insert(node.keys.begin(), left_sibling.keys.back());
                node.data_page_ids.insert(node.data_page_ids.begin(),
                                           left_sibling.data_page_ids.back());
                left_sibling.keys.pop_back();
                left_sibling.data_page_ids.pop_back();
                parent.keys[node_idx - 1] = node.keys.front();
            } else {
                node.keys.insert(node.keys.begin(), parent.keys[node_idx - 1]);
                parent.keys[node_idx - 1] = left_sibling.keys.back();
                left_sibling.keys.pop_back();
                uint32_t borrowed_child = left_sibling.children_page_ids.back();
                left_sibling.children_page_ids.pop_back();
                node.children_page_ids.insert(node.children_page_ids.begin(),
                                              borrowed_child);
                BPlusNode child = read_node(borrowed_child);
                child.parent_page_id = node.self_page_id;
                write_node(borrowed_child, child);
            }
            write_node(left_sibling_id, left_sibling);
            write_node(node_id, node);
            write_node(parent.self_page_id, parent);
            return;
        }
    }

    if (node_idx < parent.children_page_ids.size() - 1) {
        uint32_t right_sibling_id = parent.children_page_ids[node_idx + 1];
        BPlusNode right_sibling = read_node(right_sibling_id);
        if (right_sibling.keys.size() > MIN_KEYS) {
            if (node.is_leaf) {
                node.keys.push_back(right_sibling.keys.front());
                node.data_page_ids.push_back(right_sibling.data_page_ids.front());
                right_sibling.keys.erase(right_sibling.keys.begin());
                right_sibling.data_page_ids.erase(right_sibling.data_page_ids.begin());
                parent.keys[node_idx] = right_sibling.keys.front();
            } else {
                node.keys.push_back(parent.keys[node_idx]);
                parent.keys[node_idx] = right_sibling.keys.front();
                right_sibling.keys.erase(right_sibling.keys.begin());
                uint32_t borrowed_child = right_sibling.children_page_ids.front();
                right_sibling.children_page_ids.erase(right_sibling.children_page_ids.begin());
                node.children_page_ids.push_back(borrowed_child);
                BPlusNode child = read_node(borrowed_child);
                child.parent_page_id = node.self_page_id;
                write_node(borrowed_child, child);
            }
            write_node(right_sibling_id, right_sibling);
            write_node(node_id, node);
            write_node(parent.self_page_id, parent);
            return;
        }
    }

    if (node_idx > 0) {
        uint32_t left_sibling_id = parent.children_page_ids[node_idx - 1];
        BPlusNode left_sibling = read_node(left_sibling_id);
        int32_t key_from_parent = parent.keys[node_idx - 1];

        left_sibling.keys.insert(left_sibling.keys.end(), node.keys.begin(), node.keys.end());
        if (!left_sibling.is_leaf) {
            left_sibling.keys.insert(left_sibling.keys.begin() + MIN_KEYS, key_from_parent);
            left_sibling.children_page_ids.insert(left_sibling.children_page_ids.end(), node.children_page_ids.begin(), node.children_page_ids.end());
        } else {
             left_sibling.data_page_ids.insert(left_sibling.data_page_ids.end(), node.data_page_ids.begin(), node.data_page_ids.end());
             left_sibling.next_leaf_id = node.next_leaf_id;
        }
        write_node(left_sibling_id, left_sibling);
        remove_entry(node.parent_page_id, parent.keys[node_idx - 1]);
    }
}

BPlusNode BPlusTree::read_node(uint32_t page_id) {
    std::vector<char> buffer;
    file_manager.readRawPage(page_id, buffer);
    BPlusNode node;
    node.deserialize(buffer);
    node.self_page_id = page_id;
    return node;
}

void BPlusTree::write_node(uint32_t page_id, const BPlusNode& node) {
    std::vector<char> buffer;
    node.serialize(buffer);
    file_manager.writeRawPage(page_id, buffer);
}

std::vector<uint32_t> BPlusTree::get_all_data_page_ids() {
    std::vector<uint32_t> ids;
    std::unordered_set<uint32_t> seen;
    uint32_t page_id = root_page_id;
    while (true) {
        BPlusNode node = read_node(page_id);
        if (node.is_leaf) {
            break;
        }
        if (node.children_page_ids.empty()) {
            return ids;
        }
        page_id = node.children_page_ids.front();
    }

    while (page_id != 0) {
        BPlusNode leaf = read_node(page_id);
        if (!leaf.is_leaf) {
            break;
        }
        for (uint32_t pid : leaf.data_page_ids) {
            if (seen.insert(pid).second) {
                ids.push_back(pid);
            }
        }
        page_id = leaf.next_leaf_id;
    }
    return ids;
}