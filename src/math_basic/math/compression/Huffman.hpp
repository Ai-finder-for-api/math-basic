/**
 * @file Huffman.hpp
 * @brief Huffman Coding Lossless Compression.
 * 
 * Generates optimal prefix codes for a given alphabet based on frequencies.
 */
#pragma once
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <memory>
#include <cstdint>

namespace qmath {

struct HuffmanNode {
    uint8_t value;
    uint32_t freq;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;
    
    HuffmanNode(uint8_t v, uint32_t f) : value(v), freq(f), left(nullptr), right(nullptr) {}
    
    bool is_leaf() const { return !left && !right; }
};

struct HuffmanCompare {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->freq > b->freq;
    }
};

class HuffmanCoder {
private:
    std::shared_ptr<HuffmanNode> root;
    std::map<uint8_t, std::string> codes;

    void build_codes(const std::shared_ptr<HuffmanNode>& node, std::string code) {
        if (!node) return;
        if (node->is_leaf()) {
            codes[node->value] = code.empty() ? "0" : code;
            return;
        }
        build_codes(node->left, code + "0");
        build_codes(node->right, code + "1");
    }

public:
    void build_tree(const std::vector<uint8_t>& data) {
        if (data.empty()) return;
        
        std::map<uint8_t, uint32_t> freqs;
        for (uint8_t byte : data) freqs[byte]++;
        
        std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, HuffmanCompare> pq;
        for (const auto& pair : freqs) {
            pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
        }
        
        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            
            auto parent = std::make_shared<HuffmanNode>(0, left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            
            pq.push(parent);
        }
        
        root = pq.top();
        build_codes(root, "");
    }

    std::vector<uint8_t> encode(const std::vector<uint8_t>& data) {
        std::string bit_string;
        for (uint8_t byte : data) {
            bit_string += codes[byte];
        }
        
        // Pad to nearest byte
        while (bit_string.size() % 8 != 0) {
            bit_string += "0";
        }
        
        std::vector<uint8_t> encoded;
        for (size_t i = 0; i < bit_string.size(); i += 8) {
            uint8_t byte = 0;
            for (size_t j = 0; j < 8; ++j) {
                byte = (byte << 1) | (bit_string[i + j] - '0');
            }
            encoded.push_back(byte);
        }
        return encoded;
    }

    std::vector<uint8_t> decode(const std::vector<uint8_t>& encoded, size_t original_size) {
        std::vector<uint8_t> decoded;
        decoded.reserve(original_size);
        
        std::shared_ptr<HuffmanNode> current = root;
        
        for (uint8_t byte : encoded) {
            for (int i = 7; i >= 0; --i) {
                bool bit = (byte >> i) & 1;
                current = bit ? current->right : current->left;
                
                if (current && current->is_leaf()) {
                    decoded.push_back(current->value);
                    current = root;
                    if (decoded.size() == original_size) return decoded;
                }
            }
        }
        return decoded;
    }
};

} // namespace qmath
