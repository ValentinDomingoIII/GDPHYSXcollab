/*
	this is a bungee because it doesn't boucne/pull back the object higher than the anchor point
*/

#include "Bungee.hpp"

using namespace P6;

void Bungee::UpdateForce(P6Particle* particle, float time) {
	MyVector pos = particle->position;

	// vector pos->anchorPoint
	MyVector force = pos - this->anchorPoint;
	// distance of V above / lcurr
	float mag = force.magnitude();

	// only computes the spring force when the current length is greater than the rest length
	if (mag > this->restLength) {
		//		   f     =			-k			  *			|lcurr - lrest|
		float springForce = -this->springConstant * abs(mag - this->restLength);

		// get the direction from
		// the particle to the anchor point
		force = force.direction();
		force = force * springForce;
	}
	// apply the force
	particle->AddForce(force);
}
