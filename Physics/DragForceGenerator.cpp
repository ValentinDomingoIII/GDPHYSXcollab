#include "DragForceGenerator.hpp"

using namespace P6;

DragForceGenerator::DragForceGenerator(){

}

DragForceGenerator::DragForceGenerator(float _k1, float _k2){
	this->k1 = _k1;
	this->k2 = _k2;
}

void DragForceGenerator::UpdateForce(P6Particle* particle, float time){
	MyVector force = MyVector(0, 0, 0);
	MyVector currV = particle->velocity;

	float mag = currV.magnitude();

	if (mag <= 0) return;
	//Df        =   k1 * m   +   k2 * m 
	float dragF = (k1 * mag) + (k2 * mag);


	MyVector dir = currV.direction();

	particle->AddForce(dir * -dragF);
}
