#pragma once
#include "MyVector.hpp"

//Vf = (p2 - p1) / t
//p2 = Vf t + p1
//A = (Vf - Vi) / t
//Vf = Vi + At

namespace P6 {
	
	class P6Particle {

	public:

		float mass;

		float lifespan;

		float damping = 0.9f;
		
		MyVector position;

		MyVector velocity;

		MyVector acceleration;

		int i;

	protected:
		bool isDestroyed = false;
		MyVector accumelatedForce = MyVector(0, 0, 0);

	public:
		P6Particle();

		void AddForce(MyVector force);

		void ResetForce();

	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);

	public:
		void Update(float time);
		void CheckLife(float time);

		void Destroy();
		bool IsDestroyed();

	};
}

