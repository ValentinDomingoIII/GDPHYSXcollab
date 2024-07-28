#include "Cable.hpp"

using namespace P6;

void Cable::UpdateForce(P6Particle* particle, float time) {
	MyVector pos = particle->position;

	// vector pos->anchorPoint
	MyVector currentLine = pos - this->anchorPoint;
	// distance of V above / lcurr
	float currentLength = currentLine.magnitude();

	if (currentLength > this->cableLength) {
	/*	CLAMPING THE PARTICLE TO THE POSITION WHERE IT MEETS THE MAX LENGTH OF THE ROPE   */

		// get the direction from the anchorpoint to the particle due to the force
		MyVector direction = currentLine.direction();
		// store the clamped position where the max cable length is
		// positioned along the direction from the anchorpoint
		pos = this->anchorPoint + direction * this->cableLength;
		// change the particles position to the clamped position
		particle->position = pos;

	/*  ALLOWING THE PARTICLE TO STILL HAVE SOME OF ITS VELOCITY SO THAT IT WILL CONTINUE TO MOVE ALONG THE ROPE'S MAXIMUM LENGTH   */

		// store the current velocity of the particle
		MyVector velocity = particle->velocity;
		// get the component of the velocity in the direction of the force
		MyVector forceComponent = direction * velocity.scalarProduct(direction);
		// minus force component from the current velocity
		// this will remove the velocity in the direction of the force 
		// leaving the particle to move to a direction 
		// that is still within the rope's maximum length
		MyVector backVelocity = velocity - forceComponent;
		
		// set the velocity of the particle to the new velocity
		particle->velocity = backVelocity;
	}
}
