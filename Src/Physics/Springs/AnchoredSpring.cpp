#include "AnchoredSpring.hpp"

using namespace P6;

AnchoredSpring::AnchoredSpring(MyVector pos, float _springConst, float _restLen){
	this->anchorPoint = pos;
	this->springConstant = _springConst;
	this->restLength = _restLen;
}

void AnchoredSpring::UpdateForce(P6Particle* particle, float time){
	MyVector pos = particle->position;

	// vector pos->anchorPoint
	MyVector force = pos - this->anchorPoint;
	// distance of V above / lcurr
	float mag = force.magnitude();

	//		   f     =			-k			  *			|lcurr - lrest|
	float springForce = -this->springConstant * abs(mag - this->restLength);

	// get the direction from
	// the particle to the anchor point
	force = force.direction();
	force = force * springForce;

	// apply the force
	particle->AddForce(force);

}
