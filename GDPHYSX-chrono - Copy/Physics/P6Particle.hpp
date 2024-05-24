#pragma once
#include "MyVector.h"

namespace Physics {
	class P6Particle {
	public:
		float mass;

		MyVector position;
		MyVector velocity;
		MyVector acceleration;

	public:
		P6Particle();

	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);

	public:
		void Update(float time);
	};


}