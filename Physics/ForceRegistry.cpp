#include "ForceRegistry.hpp"

using namespace P6;

void ForceRegistry::Add(P6Particle* particle, ForceGenerator* generator){
	ParticleForceRegistry toAdd;

	toAdd.particle = particle;
	toAdd.generator = generator;

	this->Registry.push_back(toAdd);
}

void ForceRegistry::Remove(P6Particle* particle, ForceGenerator* generator) {
	Registry.remove_if(
		[particle, generator](ParticleForceRegistry reg) {
			return reg.particle == particle && reg.generator == generator;
		}
	);
}

void ForceRegistry::Clear(){
	Registry.clear();
}

void ForceRegistry::UpdateForces(float time){
	for (ParticleForceRegistry i : this->Registry) {
		i.generator->UpdateForce(i.particle, time);
	}
}
