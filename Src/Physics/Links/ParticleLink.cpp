#include "ParticleLink.hpp"

using namespace P6;

float ParticleLink::CurrentLength(){
	MyVector ret = this->particles[0]->position - this->particles[1]->position;
	
	return ret.magnitude();
}
