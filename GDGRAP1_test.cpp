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
#include "Camera.hpp"

#define WINDOW_HEIGHT 800.f
#define WINDOW_WIDTH 800.f

glm::vec3 cameraPos(0, 0, 0);

glm::vec3 center(0, 0, 0);

bool orthoB = true;
bool perB = false;

bool play = true;

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
void generateRandomColor(GLfloat* color) {
    color[0] = randomFloat(0.0f, 1.0f);
    color[1] = randomFloat(0.0f, 1.0f);
    color[2] = randomFloat(0.0f, 1.0f);
    color[3] = 1.0f;

}

// Function to generate random position within the window
P6::MyVector generateRandomForce() {
    P6::MyVector Force;

    Force.x = randomFloat(-4000.f, 4000.f);
    Force.y = randomFloat(1000.f, 10000.f);
    Force.z = randomFloat(-4000.f, 4000.f);

    return Force;
}

void Key_CallBack(GLFWwindow* window, //pointer to the window
    int key, //keycode of the press
    int scancode, //physical position of the press
    int action, //either press / release
    int mods) {// which modifier keys is held down

    float movementSpeed = 1.0f;

    if (key == GLFW_KEY_W) {
        cameraPos.y += movementSpeed;
    }

    if (key == GLFW_KEY_S) {
        cameraPos.y -= movementSpeed;
    }

    if (key == GLFW_KEY_A) {
        cameraPos.x -= movementSpeed;
    }

    if (key == GLFW_KEY_D) {
        cameraPos.x += movementSpeed;
    }

    if (key == GLFW_KEY_1) {
        orthoB = true;
        perB = false;
    }

    if (key == GLFW_KEY_2) {
        orthoB = false;
        perB = true;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        play = !play;
    }

    if (cameraPos.y > 89) {
        cameraPos.y = 89;
    }

    if (cameraPos.y < -89) {
        cameraPos.y = -89;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MP1-3 physics engine particle", NULL, NULL);
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
    std::string path = "3D/sphere.obj";

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

    Shader* shader = new Shader("Shaders/sample.vert", "Shaders/sample.frag");

    OrthographicCamera* ortho = new OrthographicCamera(WINDOW_HEIGHT, WINDOW_WIDTH, shader);
    PerspectiveCamera* per = new PerspectiveCamera(WINDOW_HEIGHT, WINDOW_WIDTH, shader);

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
    std::chrono::nanoseconds life(0);

    std::vector<RenderParticle*> vecRenderParticle;

    glfwSetKeyCallback(window, Key_CallBack); //calls function for updating x,y,z of the camera

    bool pause = false;
    auto lastSpawnTime = clock::now(); //timer for spawn
    int spawnedObjects = 0;  //detects alst object spawn time

    while (!glfwWindowShouldClose(window))
    {
        ortho->setCameraUse(orthoB);
        per->setCameraUse(perB);

        glm::vec3 pos;

        pos.x = cos(glm::radians(cameraPos.x)) * cos(glm::radians(cameraPos.y)) * 400.f;
        pos.y = sin(glm::radians(cameraPos.y)) * 400.f;
        pos.z = sin(glm::radians(cameraPos.x)) * cos(glm::radians(cameraPos.y)) * 400.f;

        if (ortho->getCameraUse()) {
            ortho->Update(pos, center);
        }
        if (per->getCameraUse()) {
            glm::vec3 perPos(pos.x, pos.y, -pos.z);
            per->Update(perPos, center);
        }

        if (play && pause) {
            prev_time = clock::now();
            pause = false;
        }

        if (play) {
            // Get the current time
            curr_time = clock::now();
            // Check the duration in between the last iteration
            auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
            // Set prev with the current for the next iteration
            prev_time = curr_time;

            curr_ns += dur;
            life += dur;

            if (curr_ns >= timestep)
            {
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
                auto life1 = std::chrono::duration_cast<std::chrono::milliseconds>(life);

                curr_ns -= timestep;

                // Convert ms to seconds
                pWorld.Update((float)ms.count() / 1000);


                for (std::vector<RenderParticle*>::iterator i = vecRenderParticle.begin(); i != vecRenderParticle.end();) {
                    (*i)->getParticle()->CheckLife((float)life1.count() / 1000);

                    if ((*i)->getParticle()->IsDestroyed()) {
                        i = vecRenderParticle.erase(i);
                    }
                    else {
                        i++;
                    }
                }
            }

            // spawns a new object if one second has passed since the last spawn
            auto now = clock::now(); // gets the current time
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSpawnTime); // Calculate elapsed time in miliseconds
            if (elapsed_ms.count() >= 5 && spawnedObjects < numObjects) { // Check if one second has passed and the number of objects spawned is less than numObjects
                // spawns the object
                P6::P6Particle* particle = new P6::P6Particle();
                particle->position = P6::MyVector(0, -350, 0);
                particle->mass = 1.f; // 1KG
                particle->AddForce(generateRandomForce()); // Apply some initial force
                particle->lifespan = randomFloat(1.f, 10.f);
                pWorld.forceRegistry.Add(particle, &drag);
                pWorld.AddParticle(particle);

                GLfloat color[4];

                generateRandomColor(color);

                float scale = randomFloat(2.0f, 10.f);

                Object* obj = new Object(color, shader);
                obj->scale_x = scale;
                obj->scale_y = scale;
                obj->scale_z = scale;

                RenderParticle* renderParticle = new RenderParticle(particle, obj);
                vecRenderParticle.push_back(renderParticle);

                // updates the last spawn time and increment the counter
                lastSpawnTime = now; // updates the last spawn time
                spawnedObjects++; // increments the number of objects spawned
            }

        }
        else {
            pause = true;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);

        
        for (RenderParticle* particle : vecRenderParticle) {
            particle->Draw();

            if (ortho->getCameraUse()) {
                shader->setMat4("projection", 1, ortho->getProjectionMatrix());
                shader->setMat4("view", 1, ortho->getViewMatrix());
            }

            if (per->getCameraUse()) {
                shader->setMat4("projection", 1, per->getProjectionMatrix());
                shader->setMat4("view", 1, per->getViewMatrix());
            }

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