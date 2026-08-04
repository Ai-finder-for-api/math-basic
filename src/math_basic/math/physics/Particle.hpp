/**
 * @file Particle.hpp
 * @brief Mass Aggregate Physics Engine (Particle Dynamics).
 * 
 * Implements a particle system with forces, gravity, drag, and springs.
 * Forms the basis for cloth, fluid, and soft-body simulations.
 */
#pragma once
#include "../3d/Vec3.hpp"
#include "../core/Constants.hpp"
#include <vector>
#include <cmath>

namespace qmath {

template<typename T>
class Particle {
public:
    Vec3<T> position;
    Vec3<T> velocity;
    Vec3<T> acceleration;
    T inverse_mass; // 0 means immovable
    T damping;      // Velocity drag coefficient

    Particle() : position(0,0,0), velocity(0,0,0), acceleration(0,0,0), inverse_mass(1), damping(static_cast<T>(0.99)) {}
    
    void set_mass(T mass) {
        if (mass == 0) inverse_mass = 0;
        else inverse_mass = static_cast<T>(1) / mass;
    }
    
    T get_mass() const {
        if (inverse_mass == 0) return std::numeric_limits<T>::max();
        return static_cast<T>(1) / inverse_mass;
    }

    bool is_immovable() const { return inverse_mass == 0; }

    // Integrate physics state over time delta
    void integrate(T dt) {
        if (dt <= 0 || inverse_mass == 0) return;
        
        // Update position: p = p + v*dt
        position.add_scaled_vector(velocity, dt);
        
        // Calculate accumulated acceleration
        Vec3<T> resulting_acc = acceleration;
        
        // Apply drag
        velocity *= std::pow(damping, dt);
        
        // Update velocity: v = v + a*dt
        velocity.add_scaled_vector(resulting_acc, dt);
        
        // Clear accumulated forces
        acceleration.clear();
    }
};

} // namespace qmath
