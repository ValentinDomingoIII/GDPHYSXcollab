#include "ForceGenerator.hpp"

using namespace P6;

void ForceGenerator::UpdateForce(P6Particle* p, float time){
	p->AddForce(MyVector(0, 0, 0));
}
