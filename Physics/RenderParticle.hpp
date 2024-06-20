#pragma once

#include "P6Particle.hpp"
#include "../Object.hpp"

class RenderParticle {
private:
	P6::P6Particle* PhysicsParticle;
	Object* object;

public:
	RenderParticle(P6::P6Particle* Particle, Object* object);
	void Draw();

};