#pragma once
#include "P6Particle.hpp"

namespace P6{
	class ForceGenerator{
	public:
		virtual void UpdateForce(P6Particle* p, float time);
	};
}