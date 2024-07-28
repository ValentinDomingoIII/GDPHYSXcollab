#pragma once

class Camera {
protected:
	glm::vec3 cameraPos;
	glm::vec3 viewFront;
	glm::vec3 WorldUp;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float winHeight;
	float winWidth;

	Shader* shader;

	bool inUse;

public:
	Camera(float height, float width, Shader* shader) {
		this->cameraPos = {0,0,0};
		this->viewFront = {};
		this->WorldUp = glm::vec3(0, 1.0f, 0);

		this->viewMatrix = {};
		this->projectionMatrix = {};

		this->winHeight = height;
		this->winWidth = width;

		this->inUse = false;

		this->shader = shader;
	}

	/* does the update for camera (ortho and perspective) */
	void Update(glm::vec3 pos, glm::vec3 center) {
		this->setCameraPos(pos);
		this->setProjectionMatrix();
		this->setViewMatrix(center);
	}

	glm::vec3 getCameraPos() {
		return this->cameraPos;
	}

	glm::mat4 getViewMatrix() {
		return this->viewMatrix;
	}

	glm::mat4 getProjectionMatrix() {
		return this->projectionMatrix;
	}

	void setViewMatrix(glm::vec3 viewFront) {
		this->viewMatrix = glm::lookAt(this->cameraPos, viewFront, this->WorldUp);
	}

	void setCameraUse(bool use) {
		this->inUse = use;
	}

	bool getCameraUse() {
		return this->inUse;
	}

	void setCameraPos(glm::vec3 cameraPos) {
		this->cameraPos = cameraPos;
	}

	virtual void setProjectionMatrix() = 0;

};

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float height, float width, Shader* shader) : Camera(height, width, shader) {

	}

	void setProjectionMatrix() {
		this->projectionMatrix = glm::perspective(
			glm::radians(90.f),//FOV
			this->winWidth / this->winHeight, //aspect ratio
			0.1f, //znear > 0
			1000.f //zfar
		);
	}
};

class OrthographicCamera : public Camera {
public:
	OrthographicCamera(float height, float width, Shader* shader) : Camera(height, width, shader) {

	}

	void setProjectionMatrix() {
		this->projectionMatrix = glm::ortho(
			-this->winWidth / 2.f, // left
			this->winWidth / 2.f, // right
			-this->winHeight / 2.f, // bottom
			this->winHeight / 2.f, // top
			-400.f, // z near
			1000.f // z far
		);
	}
};

