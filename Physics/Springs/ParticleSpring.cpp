#include "ParticleSpring.hpp"

using namespace P6;

ParticleSpring::ParticleSpring(P6Particle* particle, float _springConst, float _restLen){
	this->otherParticle = particle;
	this->springConstant = _springConst;
	this->restLength = _restLen;
}

void ParticleSpring::UpdateForce(P6Particle* particle, float time){

	MyVector pos = particle->position;

	MyVector force = pos - otherParticle->position;

	float mag = force.magnitude();

	float springForce = -springConstant * abs(mag - this->restLength);

	force = force.direction();

	force = force * springForce;

	particle->AddForce(force);

}

