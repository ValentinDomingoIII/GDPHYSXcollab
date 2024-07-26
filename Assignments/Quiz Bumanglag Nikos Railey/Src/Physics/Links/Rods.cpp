#include "Rods.hpp"

using namespace P6;

ParticleContact* Rods::GetContact() {
	// get the current length
	float currLen = this->CurrentLength();

	// if the current length is the same
	// as the set length, do nothing
	if (currLen == this->length) {
		return nullptr;
	}

	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = this->particles[0];
	ret->particles[1] = this->particles[1];

	// get the direction of the contact

	MyVector dir = this->particles[1]->position - this->particles[0]->position;
	dir = dir.direction();

	// if current len > set length of the rod

	if (currLen > this->length) {
		ret->contactNormal = dir;
		// depth must always be positive
		// to resolve interpenetration
		ret->depth = currLen - this->length;
	}
	else {
		ret->contactNormal = dir * -1;
		ret->depth = this->length - currLen;
	}
	// set the restituion
	// always set this to 0 for rods;
	ret->restitution = this->restitution;

	// return the contact
	return ret;
}
