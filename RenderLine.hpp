#pragma once

#include "Physics/MyVector.hpp"

using namespace P6;

class RenderLine {
private:
	MyVector P1;
	MyVector P2;

	MyVector Color;
public:
	RenderLine(MyVector p1, MyVector p2, MyVector color) {
		this->P1 = p1;
		this->P2 = p2;
		this->Color = color;
	}

	void Update(MyVector p1, MyVector p2) {
		this->P1 = p1;
		this->P2 = p2;
	}

	void Draw(Shader* shader) {
		glUseProgram(shader->ID);

		shader->setMat4("transform", 1, glm::mat4(1.0f));
	
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