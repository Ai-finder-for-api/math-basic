/**
 * @file Choice.hpp
 * @brief Random sampling and array shuffling utilities.
 * 
 * Similar to numpy.random.choice and numpy.random.shuffle.
 */
#pragma once
#include "Rand.hpp"
#include "Uniform.hpp"
#include <vector>
#include <algorithm>

namespace qmath {

// Pick a random element from a vector
template<typename T>
T choice(std::vector<T>& vec, Xoshiro256StarStar& rng) {
    if (vec.empty()) throw std::runtime_error("Cannot pick from empty vector");
    UniformInt ui(rng);
    size_t idx = static_cast<size_t>(ui(0, static_cast<int64_t>(vec.size() - 1)));
    return vec[idx];
}

// Pick N random elements from a vector (with replacement)
template<typename T>
std::vector<T> choice(std::vector<T>& vec, size_t n, Xoshiro256StarStar& rng) {
    std::vector<T> result;
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        result.push_back(choice(vec, rng));
    }
    return result;
}

// Fisher-Yates Shuffle
template<typename T>
void shuffle(std::vector<T>& vec, Xoshiro256StarStar& rng) {
    UniformInt ui(rng);
    for (size_t i = vec.size() - 1; i > 0; --i) {
        size_t j = static_cast<size_t>(ui(0, static_cast<int64_t>(i)));
        std::swap(vec[i], vec[j]);
    }
}

} // namespace qmath
