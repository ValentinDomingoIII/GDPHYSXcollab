#include "P6Particle.hpp"

using namespace P6;

P6Particle::P6Particle(){
	this->position = MyVector();
	this->acceleration = MyVector();
	this->velocity = MyVector();
	this->mass = 0.f;
}

void P6Particle::AddForce(MyVector force){
	this->accumelatedForce += force;
}

void P6Particle::ResetForce(){
	this->accumelatedForce = MyVector(0, 0, 0);
	this->acceleration = MyVector(0, 0, 0);
}

void P6Particle::UpdatePosition(float time){
	this->position = this->position + (this->velocity * time) + ((this->acceleration * time * time) * (1.0f / 2.0f));
}

void P6Particle::UpdateVelocity(float time){
	
	this->acceleration += this->accumelatedForce * (1 / mass);

	//Vf         =     Vf           +         A      *       T
	this->velocity = this->velocity + (this->acceleration * time);

	//Vfd          =     Vf         *         d^t
	this->velocity = this->velocity * powf(this->damping, time);
}

void P6Particle::Update(float time){
	this->UpdatePosition(time);
	this->UpdateVelocity(time);

	this->ResetForce();
}

void P6Particle::CheckLife(float time){
	if (this->lifespan <= time) {
		this->Destroy();
	}
}


void P6Particle::Destroy() {
	this->isDestroyed = true;
}

bool P6Particle::IsDestroyed() {
	return this->isDestroyed;
}
