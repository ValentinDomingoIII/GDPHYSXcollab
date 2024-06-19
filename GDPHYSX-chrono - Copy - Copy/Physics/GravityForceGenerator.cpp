#include "GravityForceGenerator.hpp"

using namespace P6;

GravityForceGenerator::GravityForceGenerator(const MyVector Gravity){
	this->Gravity = Gravity;
}

void GravityForceGenerator::UpdateForce(P6Particle* particle, float time){
	if (particle->mass <= 0) return;

	//      F      =       A       *      m
	MyVector force = this->Gravity * particle->mass;
	particle->AddForce(force);
}
