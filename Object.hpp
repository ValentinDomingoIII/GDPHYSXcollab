#pragma once

#include "Shaders/Shader.hpp"
#include "Physics/MyVector.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <vector>

class Object {
public:
    Shader* shader;


    float scale_x = 5;
    float scale_y = 5;
    float scale_z = 5;

    float theta = 90;

    float axis_x = 0;
    float axis_y = 1;
    float axis_z = 0;

public:
    std::string name;

    glm::vec3 pos;

    P6::MyVector avgVelocity;

    bool pushed = false;

    float magVelocity;
    float TimeBeforeStop = 0.f;

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix;
    glm::mat4 transformation_matrix;

    GLfloat* color;

public:
    Object(std::string name,GLfloat* color, glm::mat4 projectionMatrix) {
        this->name = name;
        this->color = color;
        this->projectionMatrix = projectionMatrix;
        this->shader = new Shader("Shaders/sample.vert", "Shaders/sample.frag");
    }

public:
    void draw() {
        this->transformation_matrix = glm::translate(
            this->identity_matrix,
            this->pos
        );
        this->transformation_matrix = glm::scale(
            this->transformation_matrix,
            glm::vec3(this->scale_x, this->scale_y, this->scale_z)
        );

        this->transformation_matrix = glm::rotate(
            this->transformation_matrix,
            glm::radians(this->theta),
            glm::normalize(glm::vec3(this->axis_x, this->axis_y, this->axis_z))
        );

        shader->use();

        shader->setMat4("projection", 1, this->projectionMatrix);
        shader->setMat4("transform", 1, this->transformation_matrix);
        shader->setGLfloat("color", 1, this->color);
    }
};
