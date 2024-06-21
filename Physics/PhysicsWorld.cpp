#include "PhysicsWorld.hpp"

using namespace P6;

void PhysicsWorld::AddParticle(P6Particle* toAdd){
	Particles.push_back(toAdd);

	this->forceRegistry.Add(toAdd, &this->Gravity);
}

void PhysicsWorld::Update(float time){
	//update the list first before calling
	//the updates for the particles

	this->UpdateParticleList();

	forceRegistry.UpdateForces(time);

	for (std::list<P6Particle*>::iterator p = this->Particles.begin();
		//continue looping until the end of the list
			p != Particles.end(); 
		//move to the next particle	
			p++) {

		//call the particle's update
		(*p)->Update(time);

	}
}

void PhysicsWorld::UpdateParticleList(){
	//remove all the partices in the list that
	//return true to the function below

	Particles.remove_if(
		//check aALL the particles in this list
		//if their isDestroyed flag is true
		[](P6Particle* p) {
			return p->IsDestroyed();
		}
	);
}
