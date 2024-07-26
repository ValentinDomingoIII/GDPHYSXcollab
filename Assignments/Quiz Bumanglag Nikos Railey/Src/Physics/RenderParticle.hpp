#pragma once

#include "P6Particle.hpp"
#include "../Object.hpp"

class RenderParticle {
public:
	P6::P6Particle* PhysicsParticle;
	Object* object;
	glm::vec3 scale;

public:
	RenderParticle(P6::P6Particle* Particle, Object* object);
	void Draw();
	P6::P6Particle* getParticle();
};