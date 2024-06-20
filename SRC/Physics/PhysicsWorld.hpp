#pragma once
#include <list>
#include "P6Particle.hpp"


namespace P6 {
	class PhysicsWorld {
	public:
		std::list<P6Particle*> Particles;

		void AddParticle(P6Particle* toAdd);
		void Update(float time);

	private:
		void UpdateParticleList();

	};
}
