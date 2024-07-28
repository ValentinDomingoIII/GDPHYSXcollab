#pragma once
#include "../ForceGenerator.hpp"
#include "../MyVector.hpp"

namespace P6 {
	class AnchoredSpring: public ForceGenerator {
	private:
		// this is where the spring is attached
		MyVector anchorPoint;
		// how hard your spring is
		float springConstant;
		// length of the spring at rest
		float restLength;

	public:
		AnchoredSpring(MyVector pos, float _springConst, float _restLen);
		
	public:
		void UpdateForce(P6Particle* particle, float time);
	
		MyVector getAnchorPoint() { return this->anchorPoint; }
	};
}


