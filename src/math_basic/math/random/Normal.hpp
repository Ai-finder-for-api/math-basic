/**
 * @file Normal.hpp
 * @brief Normal (Gaussian) random distribution.
 * 
 * Uses the Box-Muller transform to convert two uniform random numbers
 * into two standard normal random numbers.
 */
#pragma once
#include "Rand.hpp"
#include "Uniform.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

template<typename T>
class NormalDistribution {
private:
    Xoshiro256StarStar& rng;
    UniformReal<T> uniform;
    bool has_spare = false;
    T spare;

public:
    explicit NormalDistribution(Xoshiro256StarStar& r) : rng(r), uniform(r) {}

    // Generate a random number from N(mean, stddev^2)
    T operator()(T mean = 0, T stddev = 1) {
        if (has_spare) {
            has_spare = false;
            return mean + stddev * spare;
        }

        // Box-Muller transform
        T u1 = uniform();
        T u2 = uniform();
        
        // Avoid log(0)
        if (u1 < static_cast<T>(1e-12)) u1 = static_cast<T>(1e-12);
        
        T mag = std::sqrt(static_cast<T>(-2.0) * std::log(u1));
        T z0 = mag * std::cos(static_cast<T>(2.0) * PI_D * u2);
        T z1 = mag * std::sin(static_cast<T>(2.0) * PI_D * u2);

        spare = z1;
        has_spare = true;

        return mean + stddev * z0;
    }
};

} // namespace qmath
