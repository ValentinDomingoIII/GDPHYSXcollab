#include "MyVector.hpp"
#include <iostream>

using namespace P6;

MyVector::MyVector() : x(0), y(0), z(0) {

}

MyVector::MyVector(const float _x,
	const float _y,
	const float _z) : x(_x), y(_y), z(_z) {

}

MyVector MyVector::operator+(const MyVector v){
	return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
}

MyVector MyVector::operator-(const MyVector v) {
	return MyVector(this->x - v.x, this->y - v.y, this->z - v.z);
}

MyVector MyVector::operator*(const float v) {
	return MyVector(this->x * v, this->y * v, this->z * v);
}

void MyVector::operator += (const MyVector v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void MyVector::operator -=(const MyVector v){
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void MyVector::operator *=(const float v){
	this->x *= v;
	this->y *= v;
	this->z *= v;
}

float MyVector::magnitude(){
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

MyVector MyVector::direction(){
	return (MyVector(this->x, this->y, this->z) * (1 / this->magnitude()));
}

MyVector MyVector::componentProduct(const glm::vec3 v){
	return MyVector(this->x * v.x,
					this->y * v.y,
					this->z * v.z);
}

float MyVector::scalarProduct(const glm::vec3 v){
	return ((this->x * v.x) + (this->y * v.y) +	(this->z * v.z));
}

MyVector MyVector::vectorProduct(const glm::vec3 v){
	return MyVector((this->y * v.z) - (this->z * v.y),
					(this->z * v.x) - (this->x * v.z),
					(this->x * v.y) - (this->y * v.x));
}

