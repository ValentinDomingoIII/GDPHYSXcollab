#pragma once
#include <list>
#include "P6Particle.hpp"
#include "ForceRegistry.hpp"
#include "GravityForceGenerator.hpp"
#include "ContactResolver.hpp"
#include "Links/ParticleLink.hpp"

namespace P6 {
	class PhysicsWorld {
	protected:
		// set an arbitrary default number for
		// the max number of resolve calls
		ContactResolver contactResolver = ContactResolver(20);
	private:
		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8f, 0));
	public:
		std::vector<ParticleContact*> Contacts;
		std::list<ParticleLink*> Links;
	public:
		ForceRegistry forceRegistry;

		std::list<P6Particle*> Particles;

		void AddParticle(P6Particle* toAdd);
		void AddContact(P6Particle* p1, P6Particle* p2, float retitution, MyVector contactNormal, float depth);
		void Update(float time);

	private:
		void UpdateParticleList();
	protected:
		void GenerateContacts();
		void GetOverLap();

	};
}