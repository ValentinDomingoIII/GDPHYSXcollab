#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace Physics {

	class MyVector {
	public:
		float x, y, z;

	public:
		MyVector() : x(0), y(0), z(0) {}
		MyVector(const float _x,
			const float _y,
			const float _z) : x(_x), y(_y), z(_z) {}

	public:
		MyVector Add(MyVector b) {
			return MyVector(
				this->x + b.x,
				this->y + b.y,
				this->z + b.z);
		}

		MyVector Subtract(MyVector b) {
			return MyVector(
				this->x + b.x,
				this->y + b.y,
				this->z + b.z);
		}

		MyVector ComponentProduct(MyVector b) {
			return MyVector(
				this->x * b.x,
				this->y * b.y,
				this->z * b.z);
		}

		MyVector operator+ (const MyVector v) {
			return MyVector(
				this->x + v.x,
				this->y + v.y,
				this->z + v.z);
		}

		MyVector operator- (const MyVector v) {
			return MyVector(
				this->x - v.x,
				this->y - v.y,
				this->z - v.z);
		}

		MyVector operator* (const MyVector v) {
			return MyVector(
				this->x * v.x,
				this->y * v.y,
				this->z * v.z);
		}

		MyVector operator* (float scalar) const {
			return MyVector(x * scalar, y * scalar, z * scalar);
		}

		void operator += (const MyVector v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
		}

		void operator -= (const MyVector v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
		}

		void operator *= (const MyVector v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;
		}

		void operator*= (float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
		}

		float Magnitude() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		float DotProduct(const MyVector& v) const {
			return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
		}

		MyVector CrossProduct(const MyVector& v) const {
			return MyVector(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);
		}

		MyVector Direction() const {
			float magnitude = Magnitude();
			//do not divide by ZERO PLEASE
			if (magnitude == 0) {
				return MyVector(0, 0, 0);
			}
			return MyVector(x / magnitude, y / magnitude, z / magnitude);
		}

	public:
		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }
	};
}