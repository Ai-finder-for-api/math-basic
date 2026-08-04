/**
 * @file Vectors.hpp
 * @brief Vector constants that depend on Vec3/Vec4 types.
 *
 * Separated from Constants.hpp to avoid include cycles between
 * constants and core/vector math headers.
 */
#pragma once

#include "math/3d/Vec3.hpp"
#include "math/3d/Vec4.hpp"

namespace qmath {
namespace vectors {

inline const Vec3<float> ZERO_3D_F   = {0.0f, 0.0f, 0.0f};
inline const Vec3<float> UNIT_X_F    = {1.0f, 0.0f, 0.0f};
inline const Vec3<float> UNIT_Y_F    = {0.0f, 1.0f, 0.0f};
inline const Vec3<float> UNIT_Z_F    = {0.0f, 0.0f, 1.0f};
inline const Vec3<float> FORWARD_F   = {0.0f, 0.0f, -1.0f};
inline const Vec3<float> BACKWARD_F  = {0.0f, 0.0f, 1.0f};
inline const Vec3<float> LEFT_F      = {-1.0f, 0.0f, 0.0f};
inline const Vec3<float> RIGHT_F     = {1.0f, 0.0f, 0.0f};
inline const Vec3<float> UP_F        = {0.0f, 1.0f, 0.0f};
inline const Vec3<float> DOWN_F      = {0.0f, -1.0f, 0.0f};

inline const Vec4<float> ZERO_4D_F   = {0.0f, 0.0f, 0.0f, 0.0f};
inline const Vec4<float> UNIT_W_F    = {0.0f, 0.0f, 0.0f, 1.0f};

} // namespace vectors
} // namespace qmath
