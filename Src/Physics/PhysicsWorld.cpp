#include "PhysicsWorld.hpp"

using namespace P6;

void PhysicsWorld::AddParticle(P6Particle* toAdd){
	Particles.push_back(toAdd);

	this->forceRegistry.Add(toAdd, &this->Gravity);
}

void PhysicsWorld::AddContact(P6Particle* p1, P6Particle* p2, float restitution, MyVector contactNormal, float depth){
	ParticleContact* toAdd = new ParticleContact();

	// assign the needed variables and values
	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	this->Contacts.push_back(toAdd);
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

	this->GenerateContacts();

	// only call resolve contacts
	// when there are contacts
	if (this->Contacts.size() > 0) {
		this->contactResolver.ResolveContacts(this->Contacts, time);
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

void PhysicsWorld::GenerateContacts() {
	// clear the current list of contacts
	this->Contacts.clear();

	// check for overlaps
	this->GetOverLap();

	//iterate throught the list of links
	for (std::list<ParticleLink*>::iterator i = Links.begin();
		i != Links.end();
		i++) {
		// try and get contact for the link
		ParticleContact* contact = (*i)->GetContact();
		// if contacts exists
		// push it to the list
		if (contact != nullptr) {
			this->Contacts.push_back(contact);
		}
	}
}

void PhysicsWorld::GetOverLap() {
	// iterate through the list upto the 2nd to the last element
	for (int i = 0; i < this->Particles.size() - 1; i++) {
		std::list<P6Particle*>::iterator a = std::next(this->Particles.begin(), i);
		for(int h = i + 1; h < this->Particles.size(); h++) {
			std::list<P6Particle*>::iterator b = std::next(this->Particles.begin(), h);
			// get the vector from a -> b
			MyVector mag2Vector = (*a)->position - (*b)->position;
			// get the square magnitude
			float mag2 = mag2Vector.magnitudeSquare();

			// get the sum ofthe radius of a and b
			float rad = (*a)->radius + (*b)->radius;

			// square it
			float rad2 = pow(rad,2);

			if (mag2 <= rad2) {
				// get the direction from a->b
				MyVector dir = mag2Vector.direction();

				// get the depth of the collision
				float r = rad2 - mag2;
				float depth = sqrt(r);

				// use the lower restituion of the 2
				float restitution = fmin(
					(*a)->restitution, (*b)->restitution
				);
				
				this->AddContact(*a, *b, restitution, dir, depth);
			}
		}
	}
}
