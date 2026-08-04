/**
 * @file World.hpp
 * @brief Physics World / Registry.
 * 
 * Manages all particles and force generators, ticking the simulation.
 */
#pragma once
#include "Particle.hpp"
#include "ForceGenerator.hpp"
#include <vector>
#include <utility>

namespace qmath {

template<typename T>
class PhysicsWorld {
private:
    struct ParticleForceRegistration {
        Particle<T>* particle;
        ForceGenerator<T>* fg;
    };
    
    std::vector<Particle<T>*> particles;
    std::vector<ParticleForceRegistration> registry;

public:
    void add_particle(Particle<T>* p) {
        particles.push_back(p);
    }

    void add_force_generator(Particle<T>* p, ForceGenerator<T>* fg) {
        registry.push_back({p, fg});
    }

    void step(T dt) {
        // 1. Apply all force generators
        for (auto& reg : registry) {
            reg.fg->update_force(reg.particle, dt);
        }
        
        // 2. Integrate all particles
        for (auto* p : particles) {
            p->integrate(dt);
        }
    }
};

} // namespace qmath
