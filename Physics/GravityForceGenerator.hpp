#pragma once
#include "ForceGenerator.hpp"

namespace P6 {
	class GravityForceGenerator : public ForceGenerator{
	private:
		//Acceleration due to Gravity
		//9.8 m/s^2
		MyVector Gravity = MyVector(0, -9.8f, 0);
	
	public:
		GravityForceGenerator(const MyVector Gravity);

		void UpdateForce(P6Particle* particle, float time) override;
	};
}


