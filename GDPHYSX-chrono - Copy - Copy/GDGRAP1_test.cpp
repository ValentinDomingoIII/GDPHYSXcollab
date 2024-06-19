#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Physics/MyVector.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Physics/P6Particle.hpp"
#include "Shaders/Shader.hpp"
#include "Object.hpp"
#include "Physics/RenderParticle.hpp"
#include "Physics/DragForceGenerator.hpp"

using namespace std::chrono_literals;

// This is going to be the time between frames
constexpr std::chrono::nanoseconds timestep(16ms);

// Function to generate random float between min and max
float randomFloat(float min, float max) {
    static std::default_random_engine generator(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

// Function to generate random color
void generateRandomColor(GLfloat color[4]) {
    color[0] = randomFloat(0.0f, 1.0f);
    color[1] = randomFloat(0.0f, 1.0f);
    color[2] = randomFloat(0.0f, 1.0f);
    color[3] = 1.0f;
}

// Function to generate random position within the window
P6::MyVector generateRandomPosition(float windowWidth, float windowHeight) {
    return P6::MyVector(randomFloat(-windowWidth / 2, windowWidth / 2), randomFloat(-windowHeight / 2, windowHeight / 2), 0);
}

int main(void)
{
    float WINDOW_HEIGHT = 700.0f;
    float WINDOW_WIDTH = 700.0f;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PC01 Bumanglag, Nikos Railey", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //////////////////////////////////////////////////////////////////////////
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    std::string path = "3D/bola.obj";

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;

    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices;

    std::string warning;
    std::string error;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), attributes.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /////////////////////////////////////////////////////////////////////////
    glm::mat4 projectionMatrix = glm::ortho(
        -WINDOW_WIDTH / 2.f, // left
        WINDOW_WIDTH / 2.f, // right
        -WINDOW_HEIGHT / 2.f, // bottom
        WINDOW_HEIGHT / 2.f, // top
        -400.f, // z near
        400.f // z far
    );

    // Prompt user for number of objects
    int numObjects;
    std::cout << "Enter the number of objects to spawn: ";
    std::cin >> numObjects;

    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    P6::DragForceGenerator drag = P6::DragForceGenerator(0.f, 0.f);

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::nanoseconds stop(0);

    std::vector<RenderParticle*> vecRenderParticle;

    for (int i = 0; i < numObjects; ++i) {
        P6::P6Particle* particle = new P6::P6Particle();
        particle->position = generateRandomPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
        particle->mass = 1.f; // 1KG
        particle->AddForce(P6::MyVector(10, 1000, 0)); // Apply some initial force
        particle->lifespan = 1.0f;

        pWorld.forceRegistry.Add(particle, &drag);
        pWorld.AddParticle(particle);

        GLfloat color[4];
        generateRandomColor(color);

        Object* obj = new Object("RandomColorObject", color, projectionMatrix);
        obj->scale_x = 30.f;
        obj->scale_y = 30.f;
        obj->scale_z = 30.f;

        RenderParticle* renderParticle = new RenderParticle(particle, obj);
        vecRenderParticle.push_back(renderParticle);
    }

    while (!glfwWindowShouldClose(window))
    {
        // Get the current time
        curr_time = clock::now();
        // Check the duration in between the last iteration
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        // Set prev with the current for the next iteration
        prev_time = curr_time;

        curr_ns += dur;
        stop += dur;

        if (curr_ns >= timestep)
        {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            auto stop1 = std::chrono::duration_cast<std::chrono::milliseconds>(stop);
            curr_ns -= timestep;

            // Convert ms to seconds
            pWorld.Update((float)ms.count() / 1000);
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        for (RenderParticle* particle : vecRenderParticle) {
            particle->Draw();
            glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    char a;
    std::cout << "Enter any character to exit. . ." << std::endl;
    std::cin >> a;
    glfwTerminate();



    return 0;
}
