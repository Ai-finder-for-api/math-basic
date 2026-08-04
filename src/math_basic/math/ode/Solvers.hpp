/**
 * @file Solvers.hpp
 * @brief Numerical solvers for Ordinary Differential Equations (ODEs).
 * 
 * Implements Euler, Midpoint, and 4th-order Runge-Kutta (RK4) methods,
 * as well as Verlet integration for physics simulations.
 */
#pragma once
#include "../3d/Vec3.hpp"
#include <functional>

namespace qmath {

// Derivative function type: takes (time, state), returns derivative
template<typename T, typename State>
using DerivativeFunc = std::function<State(T, const State&)>;

// Explicit Euler Method
template<typename T, typename State>
State euler_step(T t, const State& y, T dt, const DerivativeFunc<T, State>& f) {
    return y + f(t, y) * dt;
}

// Midpoint Method (RK2)
template<typename T, typename State>
State midpoint_step(T t, const State& y, T dt, const DerivativeFunc<T, State>& f) {
    State k1 = f(t, y);
    State k2 = f(t + dt * 0.5, y + k1 * (dt * 0.5));
    return y + k2 * dt;
}

// 4th-order Runge-Kutta (RK4)
template<typename T, typename State>
State rk4_step(T t, const State& y, T dt, const DerivativeFunc<T, State>& f) {
    State k1 = f(t, y);
    State k2 = f(t + dt * 0.5, y + k1 * (dt * 0.5));
    State k3 = f(t + dt * 0.5, y + k2 * (dt * 0.5));
    State k4 = f(t + dt, y + k3 * dt);
    
    return y + (k1 + k2 * 2.0 + k3 * 2.0 + k4) * (dt / 6.0);
}

// Velocity Verlet Integration (Symplectic, good for energy conservation)
// Takes current position, current velocity, and current acceleration.
template<typename T>
Vec3<T> verlet_integrate_pos(const Vec3<T>& pos, const Vec3<T>& vel, const Vec3<T>& acc, T dt) {
    return pos + vel * dt + acc * (0.5 * dt * dt);
}

template<typename T>
Vec3<T> verlet_integrate_vel(const Vec3<T>& vel, const Vec3<T>& acc_old, const Vec3<T>& acc_new, T dt) {
    return vel + (acc_old + acc_new) * (0.5 * dt);
}

} // namespace qmath
