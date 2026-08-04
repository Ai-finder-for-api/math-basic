/**
 * @file Interp.hpp
 * @brief 1D Interpolation methods.
 * 
 * Includes Nearest, Linear, and Cubic interpolation for 1D data.
 */
#pragma once
#include "../core/NdArray.hpp"
#include "../core/Error.hpp"
#include "../core/Utils.hpp"
#include <vector>
#include <stdexcept>

namespace qmath {

enum class InterpMethod {
    NEAREST,
    LINEAR,
    CUBIC
};

inline float interpolate1d(const NdArray<float>& x, const NdArray<float>& y, float x_query, InterpMethod method = InterpMethod::LINEAR) {
    if (x.size() != y.size() || x.size() == 0) throw DimensionError("x and y must be the same non-zero size");
    if (x.size() == 1) return y[0];
    
    // Check bounds
    if (x_query <= x[0]) return y[0];
    if (x_query >= x[x.size() - 1]) return y[y.size() - 1];
    
    // Binary search to find the interval
    size_t low = 0;
    size_t high = x.size() - 1;
    size_t mid;
    
    while (high - low > 1) {
        mid = low + (high - low) / 2;
        if (x[mid] <= x_query) {
            low = mid;
        } else {
            high = mid;
        }
    }
    
    float x0 = x[low];
    float x1 = x[high];
    float y0 = y[low];
    float y1 = y[high];
    
    if (method == InterpMethod::NEAREST) {
        return (x_query - x0 < x1 - x_query) ? y0 : y1;
    }
    
    if (method == InterpMethod::LINEAR) {
        float t = (x_query - x0) / (x1 - x0);
        return y0 + t * (y1 - y0);
    }
    
    if (method == InterpMethod::CUBIC) {
        // Catmull-Rom spline interpolation
        float y_prev = (low == 0) ? y0 : y[low - 1];
        float y_next = (high == x.size() - 1) ? y1 : y[high + 1];
        
        float t = (x_query - x0) / (x1 - x0);
        float t2 = t * t;
        float t3 = t2 * t;
        
        return 0.5f * (
            (2.0f * y0) +
            (-y_prev + y1) * t +
            (2.0f * y_prev - 5.0f * y0 + 4.0f * y1 - y_next) * t2 +
            (-y_prev + 3.0f * y0 - 3.0f * y1 + y_next) * t3
        );
    }
    
    return y0; // Fallback
}

} // namespace qmath
