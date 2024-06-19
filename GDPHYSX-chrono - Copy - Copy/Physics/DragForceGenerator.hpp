#pragma once
#include "ForceGenerator.hpp"

namespace P6 {
	class DragForceGenerator : public ForceGenerator{
	private:
		//First coefficient of Friction
		float k1 = 0.74f;
		//Second coefficient of Friction
		float k2 = 0.57f;

	public:
		DragForceGenerator();
		DragForceGenerator(float _k1, float _k2);

		void UpdateForce(P6Particle* particle, float time) override;
	};
}


