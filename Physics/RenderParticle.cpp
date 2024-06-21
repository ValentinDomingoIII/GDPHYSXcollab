#include "RenderParticle.hpp"

RenderParticle::RenderParticle(P6::P6Particle* Particle, Object* object){
	this->PhysicsParticle = Particle;
	this->object = object;

	//std::cout << object->color[0] << " " << object->color[1] << " " << object->color[2] << std::endl;
}

void RenderParticle::Draw(){
	if (!this->PhysicsParticle->IsDestroyed()) {
		this->object->pos = (glm::vec3)this->PhysicsParticle->position;
		this->object->draw();
	}
}

P6::P6Particle* RenderParticle::getParticle(){

	return this->PhysicsParticle;
}
