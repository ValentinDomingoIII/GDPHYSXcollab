#include "ParticleContact.hpp"

using namespace P6;

void ParticleContact::Resolve(float time){
	// solve for the velocity after the collision
	this->ResolveVelocity(time);
	// solve for the position after collision
	this->ResolveInterpenetration(time);

}

float ParticleContact::GetSeparatingSpeed(){

	// Av
	MyVector velocity = this->particles[0]->velocity;

	if (this->particles[1]) {   
					// Av     -    Bv
		velocity -= this->particles[1]->velocity;
	} 

			//(Av - BV) dot N
	return velocity.scalarProduct(this->contactNormal);
}

void P6::ParticleContact::ResolveInterpenetration(float time){
	// if depth is less than or equal to zero, we skip
	if (this->depth <= 0) {
		return;
	}

	float totalMass = (float)1 / this->particles[0]->mass;
	if (this->particles[1]) {
		totalMass += (float)1 / this->particles[1]->mass;
	}

	// skip if the mass is 0 or negative
	if (totalMass <= 0) {
		return;
	}

	// how much to move
	float totalMoveByMass = this->depth / totalMass;
	// how much to move and where
	MyVector moveByMass = this->contactNormal * totalMoveByMass;

	MyVector P_a = moveByMass * ((float)1 / this->particles[0]->mass);
	this->particles[0]->position += P_a;

	if (this->particles[1]) {
		MyVector P_b = moveByMass * (-(float)1 / this->particles[1]->mass);
		this->particles[1]->position += P_b;
		
	}

	this->depth = 0;
}

void ParticleContact::ResolveVelocity(float time) {

	// sS
	float seperatingSpeed = this->GetSeparatingSpeed();

	// if the object is already moving away from each other
	// skip

	if (seperatingSpeed > 0) {
		return;
	}
	
	//				SAfter =		-c			*		sS					
	float newSeparatingSpeed = -this->restitution * seperatingSpeed;
	// magnitude of our delta velocity
	float deltaSpeed = newSeparatingSpeed - seperatingSpeed;

	// get the total inverse mass of the colliding particles
	// apply the impulse based on this later

	float totalMass = (float)1 / this->particles[0]->mass;
	if (this->particles[1]) {
		totalMass += (float)1 / this->particles[1]->mass;
	}

	// invalid collision  if total mass is 0 or less
	if (totalMass <= 0) {
		return;
	}

	// magnitude of impulse needed tp apply in this collision
	//			i    =		mass * deltaSpeed 
	float impulse_Mag = deltaSpeed / totalMass;

	// get the impulse vector using the contact normal
	MyVector Impulse = contactNormal * impulse_Mag;

	// change in v for particle a
	//		V	=		I			/	M
	MyVector V_a = Impulse * ((float)1 / this->particles[0]->mass);
	//					Vf		=			Vi			+	I/m
	this->particles[0]->velocity = this->particles[0]->velocity + V_a;

	if (this->particles[1]) {
		// apply impulse in the opposite direction for B
		MyVector V_b = Impulse * ((float)-1 / this->particles[1]->mass);
		//					Vf		=			Vi			+	I/m
		this->particles[1]->velocity = this->particles[1]->velocity + V_b;
	}
}
