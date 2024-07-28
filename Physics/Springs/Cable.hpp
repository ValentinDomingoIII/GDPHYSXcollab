#pragma once
#include "../ForceGenerator.hpp"
#include "../MyVector.hpp"

/*CABLE CLASS*/

namespace P6 {
	class Cable : public ForceGenerator {
	private:
		// this is where the cable is attached
		MyVector anchorPoint;
		// length of the cable at rest
		float cableLength;
	public:
		Cable(MyVector _pos, float _cableLength) : anchorPoint(_pos), cableLength(_cableLength) {}

	public:
		void UpdateForce(P6Particle* particle, float time);

		MyVector getAnchorPoint() { return this->anchorPoint; }
	};
}


