/**
 * @file Constants.hpp
 * @brief Comprehensive definitions of mathematical constants and enumerations.
 * 
 * This file provides high-precision mathematical constants for float, double,
 * and long double types. It also defines common angle modes and axis enumerations
 * used throughout the math_basic library.
 */
#pragma once
#include <limits>

namespace qmath {

// -----------------------------------------------------------------
// Mathematical Constants (Float)
// -----------------------------------------------------------------
constexpr float PI_F = 3.14159265358979323846f;
constexpr float TWO_PI_F = 2.0f * PI_F;
constexpr float HALF_PI_F = 0.5f * PI_F;
constexpr float QUARTER_PI_F = 0.25f * PI_F;
constexpr float INV_PI_F = 1.0f / PI_F;
constexpr float INV_TWO_PI_F = 1.0f / TWO_PI_F;
constexpr float E_F = 2.71828182845904523536f;
constexpr float SQRT_2_F = 1.41421356237309504880f;
constexpr float INV_SQRT_2_F = 0.70710678118654752440f;
constexpr float EPSILON_F = std::numeric_limits<float>::epsilon();
constexpr float INFINITY_F = std::numeric_limits<float>::infinity();
constexpr float NEG_INFINITY_F = -std::numeric_limits<float>::infinity();
constexpr float DEG_TO_RAD_F = PI_F / 180.0f;
constexpr float RAD_TO_DEG_F = 180.0f / PI_F;

// -----------------------------------------------------------------
// Mathematical Constants (Double)
// -----------------------------------------------------------------
constexpr double PI_D = 3.14159265358979323846;
constexpr double TWO_PI_D = 2.0 * PI_D;
constexpr double HALF_PI_D = 0.5 * PI_D;
constexpr double QUARTER_PI_D = 0.25 * PI_D;
constexpr double INV_PI_D = 1.0 / PI_D;
constexpr double INV_TWO_PI_D = 1.0 / TWO_PI_D;
constexpr double E_D = 2.71828182845904523536;
constexpr double SQRT_2_D = 1.41421356237309504880;
constexpr double INV_SQRT_2_D = 0.70710678118654752440;
constexpr double EPSILON_D = std::numeric_limits<double>::epsilon();
constexpr double INFINITY_D = std::numeric_limits<double>::infinity();
constexpr double NEG_INFINITY_D = -std::numeric_limits<double>::infinity();
constexpr double DEG_TO_RAD_D = PI_D / 180.0;
constexpr double RAD_TO_DEG_D = 180.0 / PI_D;

// -----------------------------------------------------------------
// Angle Modes
// -----------------------------------------------------------------
enum class AngleMode {
    RADIANS,
    DEGREES
};

// -----------------------------------------------------------------
// Axis Enumerations (for 3D Math)
// -----------------------------------------------------------------
enum class Axis {
    X = 0,
    Y = 1,
    Z = 2,
    W = 3
};

// -----------------------------------------------------------------
// Common 3D Vectors (Forward declaration of struct for constants)
// -----------------------------------------------------------------
template<typename T>
struct Vec3;
template<typename T>
struct Vec4;

namespace vectors {
    // Standard basis vectors (float)
    constexpr Vec3<float> ZERO_3D_F = {0.0f, 0.0f, 0.0f};
    constexpr Vec3<float> UNIT_X_F = {1.0f, 0.0f, 0.0f};
    constexpr Vec3<float> UNIT_Y_F = {0.0f, 1.0f, 0.0f};
    constexpr Vec3<float> UNIT_Z_F = {0.0f, 0.0f, 1.0f};
    constexpr Vec3<float> FORWARD_F = {0.0f, 0.0f, -1.0f};
    constexpr Vec3<float> BACKWARD_F = {0.0f, 0.0f, 1.0f};
    constexpr Vec3<float> LEFT_F = {-1.0f, 0.0f, 0.0f};
    constexpr Vec3<float> RIGHT_F = {1.0f, 0.0f, 0.0f};
    constexpr Vec3<float> UP_F = {0.0f, 1.0f, 0.0f};
    constexpr Vec3<float> DOWN_F = {0.0f, -1.0f, 0.0f};

    constexpr Vec4<float> ZERO_4D_F = {0.0f, 0.0f, 0.0f, 0.0f};
    constexpr Vec4<float> UNIT_W_F = {0.0f, 0.0f, 0.0f, 1.0f};
}

} // namespace qmath
