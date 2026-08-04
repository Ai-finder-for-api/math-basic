/**
 * @file Uniform.hpp
 * @brief Uniform random distributions.
 * 
 * Generates floating-point numbers uniformly in [0, 1) and [min, max),
 * as well as integers uniformly in [min, max].
 */
#pragma once
#include "Rand.hpp"

namespace qmath {

template<typename T>
class UniformReal {
private:
    Xoshiro256StarStar& rng;
public:
    explicit UniformReal(Xoshiro256StarStar& r) : rng(r) {}

    // Generate a random float/double in [0, 1)
    T operator()() {
        // 53 bits of precision for double, 24 for float
        uint64_t bits = rng.next();
        if constexpr (std::is_same_v<T, float>) {
            return static_cast<T>(bits >> 40) * static_cast<T>(1.0f / (1ULL << 24));
        } else {
            return static_cast<T>(bits >> 11) * static_cast<T>(1.0 / (1ULL << 53));
        }
    }

    // Generate a random float/double in [min, max)
    T operator()(T min, T max) {
        return min + (*this)() * (max - min);
    }
};

class UniformInt {
private:
    Xoshiro256StarStar& rng;
public:
    explicit UniformInt(Xoshiro256StarStar& r) : rng(r) {}

    // Generate a random integer in [min, max] (inclusive)
    int64_t operator()(int64_t min, int64_t max) {
        if (min > max) std::swap(min, max);
        uint64_t range = static_cast<uint64_t>(max - min) + 1;
        // Modulo method (slight bias for non-power-of-2 ranges, but very fast)
        return min + static_cast<int64_t>(rng.next() % range);
    }
};

} // namespace qmath
