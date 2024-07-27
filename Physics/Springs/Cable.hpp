#pragma once
#include "../ForceGenerator.hpp"
#include "../MyVector.hpp"

namespace P6 {
	class Cable : public ForceGenerator {
	private:
		// this is where the spring is attached
		MyVector anchorPoint;
		// how hard your spring is
		float springConstant = 1.f;
		// length of the spring at rest
		float cableLength;
	public:
		Cable(MyVector _pos, float _cableLength) : anchorPoint(_pos), cableLength(_cableLength) {}

	public:
		void UpdateForce(P6Particle* particle, float time);

		MyVector getAnchorPoint() { return this->anchorPoint; }
	};
}


