#pragma once

#include "P6Particle.hpp"

namespace P6 {
	class ParticleContact {
	public:
		// interpenetation depth
		float depth;
		// the particles in contact
		P6Particle* particles[2];
		// holds the coefficient of restitution
		float restitution;
		// contact normal of this collision
		MyVector contactNormal;

	public:
		// resolve this contact
		void Resolve(float time);
		// get the separating speed of this collision
		float GetSeparatingSpeed();
	protected:
		void ResolveInterpenetration(float time);
		// calculates the new velocities of the collided objects
		void ResolveVelocity(float time);
	};
}


