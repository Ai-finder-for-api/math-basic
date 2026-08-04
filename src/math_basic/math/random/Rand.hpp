/**
 * @file Rand.hpp
 * @brief High-performance Random Number Generation Engine (Xoshiro256**).
 * 
 * Xoshiro256** is a modern, extremely fast pseudo-random number generator
 * with a state space of 256 bits. It is used to generate high-quality
 * 64-bit unsigned integers.
 */
#pragma once
#include <cstdint>
#include <array>
#include <random>

namespace qmath {

class Xoshiro256StarStar {
private:
    std::array<uint64_t, 4> state;

    static inline uint64_t splitmix64(uint64_t& z) {
        z += 0x9e3779b97f4a7c15ULL;
        uint64_t result = z;
        result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9ULL;
        result = (result ^ (result >> 27)) * 0x94d049bb133111ebULL;
        return result ^ (result >> 31);
    }

    static inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

public:
    // Constructor with default seed from random_device
    Xoshiro256StarStar() {
        std::random_device rd;
        uint64_t seed = (static_cast<uint64_t>(rd()) << 32) | rd();
        state[0] = splitmix64(seed);
        state[1] = splitmix64(seed);
        state[2] = splitmix64(seed);
        state[3] = splitmix64(seed);
    }

    // Constructor with explicit seed
    explicit Xoshiro256StarStar(uint64_t seed) {
        state[0] = splitmix64(seed);
        state[1] = splitmix64(seed);
        state[2] = splitmix64(seed);
        state[3] = splitmix64(seed);
    }

    // Generate next 64-bit unsigned integer
    uint64_t next() {
        const uint64_t result = rotl(state[1] * 5, 7) * 9;
        const uint64_t t = state[1] << 17;

        state[2] ^= state[0];
        state[3] ^= state[1];
        state[1] ^= state[2];
        state[0] ^= state[3];

        state[2] ^= t;
        state[3] = rotl(state[3], 45);

        return result;
    }

    // Seed the generator
    void seed(uint64_t seed) {
        state[0] = splitmix64(seed);
        state[1] = splitmix64(seed);
        state[2] = splitmix64(seed);
        state[3] = splitmix64(seed);
    }
};

} // namespace qmath
