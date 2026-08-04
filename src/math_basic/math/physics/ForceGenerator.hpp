/**
 * @file ForceGenerator.hpp
 * @brief Generates forces for physics objects.
 * 
 * Implements springs, bungees, buoyancy, and gravity.
 */
#pragma once
#include "Particle.hpp"
#include "../3d/Vec3.hpp"
#include "../core/Utils.hpp"
#include <vector>

namespace qmath {

template<typename T>
class ForceGenerator {
public:
    virtual void update_force(Particle<T>* particle, T dt) = 0;
    virtual ~ForceGenerator() {}
};

// Gravity Force
template<typename T>
class GravityForce : public ForceGenerator<T> {
    Vec3<T> gravity;
public:
    explicit GravityForce(const Vec3<T>& g) : gravity(g) {}
    void update_force(Particle<T>* p, T dt) override {
        if (!p->is_immovable()) {
            p->acceleration += gravity * p->inverse_mass;
        }
    }
};

// Drag Force
template<typename T>
class DragForce : public ForceGenerator<T> {
    T k1; // Linear drag
    T k2; // Quadratic drag
public:
    DragForce(T linear, T quadratic) : k1(linear), k2(quadratic) {}
    void update_force(Particle<T>* p, T dt) override {
        Vec3<T> force = p->velocity;
        T speed = force.length();
        if (speed == 0) return;
        
        T drag_coeff = k1 * speed + k2 * speed * speed;
        Vec3<T> drag_dir = force.normalized() * (-drag_coeff);
        p->acceleration += drag_dir * p->inverse_mass;
    }
};

// Spring Force (Hooke's Law)
template<typename T>
class SpringForce : public ForceGenerator<T> {
    Particle<T>* other;
    T spring_constant;
    T rest_length;
public:
    SpringForce(Particle<T>* o, T k, T rest) : other(o), spring_constant(k), rest_length(rest) {}
    void update_force(Particle<T>* p, T dt) override {
        Vec3<T> force = p->position - other->position;
        T current_length = force.length();
        if (current_length == 0) return;
        
        T extension = current_length - rest_length;
        force = force.normalized() * (-spring_constant * extension);
        p->acceleration += force * p->inverse_mass;
    }
};

// Anchored Spring (attached to a fixed point in space)
template<typename T>
class AnchoredSpring : public ForceGenerator<T> {
    Vec3<T> anchor;
    T spring_constant;
    T rest_length;
public:
    AnchoredSpring(const Vec3<T>& a, T k, T rest) : anchor(a), spring_constant(k), rest_length(rest) {}
    void update_force(Particle<T>* p, T dt) override {
        Vec3<T> force = p->position - anchor;
        T current_length = force.length();
        if (current_length == 0) return;
        
        T extension = current_length - rest_length;
        force = force.normalized() * (-spring_constant * extension);
        p->acceleration += force * p->inverse_mass;
    }
};

} // namespace qmath
