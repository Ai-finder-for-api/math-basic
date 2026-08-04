/**
 * @file PerlinNoise.hpp
 * @brief Deterministic Perlin Noise generation (2D and 3D).
 * 
 * Used for procedural terrain, textures, and organic motion.
 * Implements Ken Perlin's improved noise algorithm with a custom seed.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include <vector>
#include <numeric>
#include <random>
#include <cmath>

namespace qmath {

class PerlinNoise {
private:
    std::vector<int> permutation;

    double fade(double t) const {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double lerp(double t, double a, double b) const {
        return a + t * (b - a);
    }

    double grad(int hash, double x, double y, double z) const {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

public:
    explicit PerlinNoise(unsigned int seed = 0) {
        permutation.resize(512);
        std::vector<int> p(256);
        std::iota(p.begin(), p.end(), 0);
        
        std::mt19937 gen(seed);
        std::shuffle(p.begin(), p.end(), gen);
        
        for (int i = 0; i < 256; ++i) {
            permutation[i] = p[i];
            permutation[i + 256] = p[i];
        }
    }

    // 3D Perlin Noise
    double noise(double x, double y, double z) const {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;
        
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);
        
        double u = fade(x);
        double v = fade(y);
        double w = fade(z);
        
        int A = permutation[X] + Y;
        int AA = permutation[A] + Z;
        int AB = permutation[A + 1] + Z;
        int B = permutation[X + 1] + Y;
        int BA = permutation[B] + Z;
        int BB = permutation[B + 1] + Z;
        
        return lerp(w, 
            lerp(v,
                lerp(u, grad(permutation[AA], x, y, z), grad(permutation[BA], x-1, y, z)),
                lerp(u, grad(permutation[AB], x, y-1, z), grad(permutation[BB], x-1, y-1, z))
            ),
            lerp(v,
                lerp(u, grad(permutation[AA+1], x, y, z-1), grad(permutation[BA+1], x-1, y, z-1)),
                lerp(u, grad(permutation[AB+1], x, y-1, z-1), grad(permutation[BB+1], x-1, y-1, z-1))
            )
        );
    }

    // 2D Perlin Noise
    double noise(double x, double y) const {
        return noise(x, y, 0.0);
    }

    // Fractal Brownian Motion (fBm) for multi-octave noise
    double fbm(double x, double y, double z, int octaves = 4, double persistence = 0.5, double lacunarity = 2.0) const {
        double total = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;
        double max_value = 0.0;
        
        for (int i = 0; i < octaves; ++i) {
            total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
            max_value += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }
        
        return total / max_value;
    }
};

} // namespace qmath
