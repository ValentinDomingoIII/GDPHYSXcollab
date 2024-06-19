#include "RenderParticle.hpp"

RenderParticle::RenderParticle(P6::P6Particle* Particle, Object* object){
	this->PhysicsParticle = Particle;
	this->object = object;
}

void RenderParticle::Draw(){
	if (!this->PhysicsParticle->IsDestroyed()) {
		this->object->pos = (glm::vec3)this->PhysicsParticle->position;
		this->object->draw();
	}
}
