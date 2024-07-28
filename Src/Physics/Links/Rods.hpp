#pragma once
#include "ParticleLink.hpp"

namespace P6 {
	class Rods: public ParticleLink {
	public:
		// length of the rod  (in meters)
		float length = 1;

		// always set restitution to 0
		// since a rod has no bounciness

		float restitution = 0;

		// override the link's GetContact()
		ParticleContact* GetContact() override;
	};
}


