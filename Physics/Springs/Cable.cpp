#include "Cable.hpp"

using namespace P6;

void Cable::UpdateForce(P6Particle* particle, float time) {
	MyVector pos = particle->position;

	// vector pos->anchorPoint
	MyVector force = pos - this->anchorPoint;
	// distance of V above / lcurr
	float mag = force.magnitude();

	if (mag > this->cableLength) {
		// Clamp the particle position to be exactly at the max length
		MyVector direction = force.direction();
		pos = this->anchorPoint + direction * this->cableLength;
		particle->position = pos;

		// store the current velocity of the particle
		MyVector velocity = particle->velocity;

		// 
		MyVector normalComponent = direction * velocity.scalarProduct(direction);
		//
		MyVector backVelocity = velocity - normalComponent;
		// set the the velocity of the particle
		particle->velocity = backVelocity;
	}

	//		   f     =			-k			  *			|lcurr - lrest|
	float springForce = -this->springConstant * abs(mag - this->cableLength);

	// get the direction from
	// the particle to the anchor point
	force = force.direction();
	force = force * springForce;

	// apply the force
	particle->AddForce(force);
}
