#include "P6Particle.hpp"

using namespace P6;

P6Particle::P6Particle() {

}

void P6Particle::UpdatePosition(float time) {
	this->position = this->position + (this->velocity * time) + ((this->acceleration * time * time) * (1.0f / 2.0f));
}

void P6Particle::UpdateVelocity(float time) {
	this->velocity = this->velocity + this->acceleration * time;
}

void P6Particle::Update(float time) {
	this->UpdateVelocity(time);
	this->UpdatePosition(time);
}

void P6Particle::Destroy() {
	this->isDestroyed = true;
}

bool P6Particle::Destroyed() {
	return this->isDestroyed;
}


