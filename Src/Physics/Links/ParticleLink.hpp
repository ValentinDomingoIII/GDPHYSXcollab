#pragma once
#include "../P6Particle.hpp"
#include "../ParticleContact.hpp"

namespace P6 {
	class ParticleLink {
	public:
		// particles that are linked
		P6Particle* particles[2];
		// returns a contact if particles violate the condition of the link
		virtual ParticleContact* GetContact() { return nullptr; };

	protected:
		// distance between the 2 particles
		// used mainly for interpenetration
		float CurrentLength();
	};
}


