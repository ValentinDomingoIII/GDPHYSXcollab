#pragma once
#include "MyVector.h"

namespace P6 {
	class P6Particle {
	public:
		float mass;

		MyVector position;
		MyVector velocity;
		MyVector acceleration;
	protected:
		bool isDestroyed = false;

	public:
		P6Particle();

	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);

	public:
		void Update(float time);

		void Destroy();
		bool Destroyed();


	};
}