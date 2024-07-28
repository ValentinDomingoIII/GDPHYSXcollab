#pragma once
#include <vector>
#include "ParticleContact.hpp"

namespace P6 {
	class ContactResolver {
	protected:
		// current number of resolve contacts
		unsigned current_iterations = 0;
	public:
		// max number of resolve calls
		unsigned max_iterations;
	public:
		ContactResolver(unsigned _maxIterations);
	public:
		void ResolveContacts(std::vector<ParticleContact*> contacts, float time);
	};
}


