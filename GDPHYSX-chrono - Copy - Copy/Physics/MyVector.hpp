#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {

	class MyVector {
	public:
		float x, y, z;

	public:
		MyVector();
		MyVector(const float _x,
			const float _y,
			const float _z);

	public:
		MyVector operator + (const MyVector v);
		MyVector operator - (const MyVector v);
		MyVector operator * (const float v);

		void operator += (const MyVector v);
		void operator -= (const MyVector v);
		void operator *= (const float v);

	public:
		float magnitude();
		MyVector direction();
		MyVector componentProduct(const glm::vec3 v);
		float scalarProduct(const glm::vec3 v);
		MyVector vectorProduct(const glm::vec3 v);

	public:
		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }
	};
}
