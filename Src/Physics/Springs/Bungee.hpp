#pragma once
#include "../ForceGenerator.hpp"

namespace P6 {
	class Bungee : public ForceGenerator{
	private:
		// point of the start of the rope
		MyVector anchorPoint = MyVector(-300,200,0);
		// how hard the rope is
		float springConstant = 10.f;
		// actual length of the rope when at rest (not stretched)
		float restLength = 200.f;
	public:
		void UpdateForce(P6Particle* particle, float time);
		
		MyVector getanchorPoint() { return this->anchorPoint; }
	};
}


