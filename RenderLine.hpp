#pragma once

#include "Physics/MyVector.hpp"

using namespace P6;

class RenderLine {
private:
	MyVector P1;
	MyVector P2;

	glm::mat4 projectionMatrix;

	MyVector Color;
public:
	RenderLine(MyVector p1, MyVector p2, MyVector color) {
		this->P1 = p1;
		this->P2 = p2;
		this->Color = color;
	}

	void Update(MyVector p1, MyVector p2, glm::mat4 projectionMatrix) {
		this->P1 = p1;
		this->P2 = p2;
		this->projectionMatrix = projectionMatrix;

	}

	void Draw(Shader* shader, glm::mat4 view) {
		glUseProgram(shader->ID);

		shader->setMat4("transform", 1, glm::mat4(1.0f));
		shader->setMat4("view", 1, view);
		shader->setMat4("projection", 1, this->projectionMatrix);
	
		glm::vec4 d1 = glm::vec4(
			this->P1.x, this->P1.y, this->P1.z, 1.0f
		);

		glm::vec4 d2 = glm::vec4(
			this->P2.x, this->P2.y, this->P2.z, 1.0f
		);

		glBegin(GL_LINES);
		glVertex3f(d1.x, d1.y, d1.z);
		glVertex3f(d2.x, d2.y, d2.z);
		glEnd();
	}
};