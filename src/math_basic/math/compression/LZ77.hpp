/**
 * @file LZ77.hpp
 * @brief LZ77 Sliding Window Compression Algorithm.
 * 
 * The foundation of DEFLATE (used in ZIP and PNG). Compresses data by
 * replacing repeated sequences with references to previous occurrences.
 */
#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

namespace qmath {

struct LZ77Token {
    uint16_t offset;
    uint16_t length;
    uint8_t next_char;
};

inline std::vector<LZ77Token> lz77_compress(const std::vector<uint8_t>& data, size_t window_size = 32768, size_t lookahead_size = 258) {
    std::vector<LZ77Token> tokens;
    size_t pos = 0;
    
    while (pos < data.size()) {
        size_t best_length = 0;
        size_t best_offset = 0;
        
        size_t window_start = (pos > window_size) ? (pos - window_size) : 0;
        
        for (size_t i = window_start; i < pos; ++i) {
            size_t match_len = 0;
            while (match_len < lookahead_size && 
                   pos + match_len < data.size() && 
                   data[i + match_len] == data[pos + match_len]) {
                match_len++;
            }
            
            if (match_len > best_length) {
                best_length = match_len;
                best_offset = pos - i;
            }
        }
        
        LZ77Token token;
        token.offset = static_cast<uint16_t>(best_offset);
        token.length = static_cast<uint16_t>(best_length);
        token.next_char = (pos + best_length < data.size()) ? data[pos + best_length] : 0;
        
        tokens.push_back(token);
        pos += best_length + 1;
    }
    
    return tokens;
}

inline std::vector<uint8_t> lz77_decompress(const std::vector<LZ77Token>& tokens) {
    std::vector<uint8_t> data;
    for (const auto& token : tokens) {
        if (token.length > 0) {
            size_t start = data.size() - token.offset;
            for (size_t i = 0; i < token.length; ++i) {
                data.push_back(data[start + i]);
            }
        }
        data.push_back(token.next_char);
    }
    return data;
}

} // namespace qmath
