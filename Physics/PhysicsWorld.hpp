#pragma once
#include <list>
#include "P6Particle.hpp"
#include "ForceRegistry.hpp"
#include "GravityForceGenerator.hpp"

namespace P6 {
	class PhysicsWorld {
	private:
		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -0, 0));

	public:
		ForceRegistry forceRegistry;

		std::list<P6Particle*> Particles;

		void AddParticle(P6Particle* toAdd);
		void Update(float time);

	private:
		void UpdateParticleList();

	};
}