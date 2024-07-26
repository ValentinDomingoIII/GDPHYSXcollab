#pragma once
#include "../ForceGenerator.hpp"
#include "../P6Particle.hpp"

namespace P6 {
	class ParticleSpring: public ForceGenerator {
	private: 
		P6Particle* otherParticle;

		float springConstant;

		float restLength;

	public:
		ParticleSpring(P6Particle* particle, float _springConst, float _restLen);
		
		void UpdateForce(P6Particle* particle, float time);

		MyVector getAnchorPoint() { return this->otherParticle->position; }
	};
}


