/*
	this is a chain because it stops when it reaches the resting length of the rope
	and doesn't bounce/pull back the object back like a spring or the bungee
*/


#include "Chain.hpp"

using namespace P6;

void Chain::UpdateForce(P6Particle* particle, float time) {
	MyVector pos = particle->position;

	// vector    Pos ->  anchor point of the chain
	MyVector force = pos - this->anchorPoint;
	// distance of of the vector above
	float mag = force.magnitude();
	
	// if the current length of the chain is greater than or equal then the rest length
	if (mag >= this->restLength) {
		// neutralize the gravity
		force = (MyVector(0 ,9.8f ,0) + particle->velocity * -1.f) * particle->mass;
	}

	particle->AddForce(force);
}
