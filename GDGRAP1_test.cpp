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
#include "Physics/RenderParticle.hpp"
#include "Physics/DragForceGenerator.hpp"
#include "Physics/ParticleContact.hpp"
#include "Physics/Springs/AnchoredSpring.hpp"
#include "Physics/Springs/ParticleSpring.hpp"
#include "Physics/Springs/Cable.hpp"
#include "Physics/Springs/Bungee.hpp"
#include "Physics/Springs/Chain.hpp"
#include "Physics/Links/Rods.hpp"
#include "Shaders/Shader.hpp"
#include "Object.hpp"
#include "RenderLine.hpp"

#include "Camera.hpp"

// use the namespace so it doesn't need to be typed later on
using namespace P6;

#define WINDOW_HEIGHT 800.f
#define WINDOW_WIDTH 800.f

// set the starting position of the camera
glm::vec3 cameraPos(100, 0, 400);

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
MyVector generateRandomForce() {
    MyVector Force;

    Force.x = randomFloat(-4000.f, 4000.f);
    Force.y = randomFloat(1000.f, 10000.f);
    Force.z = randomFloat(-4000.f, 4000.f);

    return Force;
}

float forceX, forceY, forceZ;

//early declration
P6Particle* particle1 = new P6Particle();
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

    //checks if space key is RELEASED to prevent 'hold' press

    if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        play = !play;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        particle1->AddForce(MyVector(forceX, forceY, forceZ));
    }

    //limits camera rotation on y
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
    //for user input
    float scale;
    float distance;
    float length;
    float yGravity;

    std::cout << "Enter the length of the cable: ";
    std::cin >> length;
    std::cout << "Enter the scale for the objects: ";
    std::cin >> scale;
    std::cout << "Enter the distance for the objects: ";
    std::cin >> distance;
    std::cout << "Enter the gravity strength: ";
    std::cin >> yGravity;
    std::cout << "Enter the force X: ";
    std::cin >> forceX;
    std::cout << "Enter the force Y: ";
    std::cin >> forceY;
    std::cout << "Enter the force Z: ";
    std::cin >> forceZ;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MP PHASE 2 - 1", NULL, NULL);
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

    // set the shader to be used 
    Shader* shader = new Shader("Shaders/sample.vert", "Shaders/sample.frag");

    // set the ortho and perspective
    OrthographicCamera* ortho = new OrthographicCamera(WINDOW_HEIGHT, WINDOW_WIDTH, shader);
    PerspectiveCamera* per = new PerspectiveCamera(WINDOW_HEIGHT, WINDOW_WIDTH, shader);

    PhysicsWorld pWorld = PhysicsWorld();
    pWorld.Gravity = MyVector(0, yGravity, 0);
    DragForceGenerator drag = DragForceGenerator();

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::nanoseconds life(0);

    std::vector<RenderParticle*> vecRenderParticle;

    
    particle1->position = MyVector(0-(distance*2), 150, 0);
    particle1->mass = 50.f; // 1KG
   //particle1->AddForce(MyVector(10, -10, 0) * 000.0f); // Apply some initial force
    particle1->lifespan = 1000.f;
    pWorld.forceRegistry.Add(particle1, &drag);
    pWorld.AddParticle(particle1);

    GLfloat red[] = { 1,0,0,1 };

    Object* obj1 = new Object(red, shader);
    obj1->scale_x = scale;
    obj1->scale_y = scale;
    obj1->scale_z = scale;

    particle1->radius = scale;


    RenderParticle* renderParticle1 = new RenderParticle(particle1, obj1);
    renderParticle1->scale = glm::vec3(particle1->radius, particle1->radius, particle1->radius);
    vecRenderParticle.push_back(renderParticle1);
    //////////////2nd ball////////////////////////////
    P6Particle* particle2 = new P6Particle();
    particle2->position = MyVector(0-distance, 150, 0);
    particle2->mass = 50.f; // 1KG
    particle2->AddForce(MyVector(-10, -10, 0) * 0000.0f); // Apply some initial force
    particle2->lifespan = 1000.f;
    pWorld.forceRegistry.Add(particle2, &drag);
    pWorld.AddParticle(particle2);

    GLfloat blue[] = { 0,0,1,1 };

    Object* obj2 = new Object(blue, shader);
    obj2->scale_x = scale;
    obj2->scale_y = scale;
    obj2->scale_z = scale;

    particle2->radius = scale;
 

    RenderParticle* renderParticle2 = new RenderParticle(particle2, obj2);
    renderParticle2->scale = glm::vec3(particle2->radius, particle2->radius, particle2->radius);
    vecRenderParticle.push_back(renderParticle2);
    /////////////////////3RD OBJECT//////////////////////////////
         
    
    P6Particle* particle3 = new P6Particle();
    particle3->position = MyVector(0, 150, 0);
    particle3->mass = 50.f; // 1KG
    particle3->AddForce(MyVector(-10, -10, 0) * 0000.0f); // Apply some initial force
    particle3->lifespan = 1000.f;
    pWorld.forceRegistry.Add(particle3, &drag);
    pWorld.AddParticle(particle3);

    GLfloat velvet[] = { 0.545f, 0.0f, 0.545f, 1.0f };

    Object* obj3 = new Object(velvet, shader);
    obj3->scale_x = scale;
    obj3->scale_y = scale;
    obj3->scale_z = scale;

    particle3->radius = scale;


    RenderParticle* renderParticle3 = new RenderParticle(particle3, obj3);
    renderParticle3->scale = glm::vec3(particle3->radius, particle3->radius, particle3->radius);
    vecRenderParticle.push_back(renderParticle3);
    //////////////////////////////4TH OBJECT /////////////////////

    P6Particle* particle4 = new P6Particle();
    particle4->position = MyVector(0+distance, 150, 0);
    particle4->mass = 50.f; // 1KG
    particle4->AddForce(MyVector(-10, -10, 0) * 0000.0f); // Apply some initial force
    particle4->lifespan = 1000.f;
    pWorld.forceRegistry.Add(particle4, &drag);
    pWorld.AddParticle(particle4);

    GLfloat Lavender[] = { 0.9, 0.9, 0.98, 1.0 };

    Object* obj4 = new Object(Lavender, shader);
    obj4->scale_x = scale;
    obj4->scale_y = scale;
    obj4->scale_z = scale;

    particle4->radius = scale;

    RenderParticle* renderParticle4 = new RenderParticle(particle4, obj4);
    renderParticle4->scale = glm::vec3(particle4->radius, particle4->radius, particle4->radius);
    vecRenderParticle.push_back(renderParticle4);
/////////////////////////////////5th OBJECT ////////////////////
    P6Particle* particle5 = new P6Particle();
    particle5->position = MyVector(0+(distance*2), 150, 0);
    particle5->mass = 50.f; // 1KG
    particle5->AddForce(MyVector(-10, -10, 0) * 0000.0f); // Apply some initial force
    particle5->lifespan = 1000.f;
    pWorld.forceRegistry.Add(particle5, &drag);
    pWorld.AddParticle(particle5);

    GLfloat violet[] = { 0.933f, 0.510f, 0.933f, 1.0f };

    Object* obj5 = new Object(violet, shader);
    obj5->scale_x = scale;
    obj5->scale_y = scale;
    obj5->scale_z = scale;
    particle5->radius = scale;
    RenderParticle* renderParticle5 = new RenderParticle(particle5, obj5);
    renderParticle5->scale = glm::vec3(particle5->radius, particle5->radius, particle5->radius);
    vecRenderParticle.push_back(renderParticle5);

    ///////////////////class lines///////////////////////
    Cable* cable1 = new Cable(MyVector(0-(distance*2), 160, 0), length);
    pWorld.forceRegistry.Add(particle1, cable1);

    Cable* cable2 = new Cable(MyVector(0-distance, 160, 0), length);
    pWorld.forceRegistry.Add(particle2, cable2);

    Cable* cable3 = new Cable(MyVector(0, 160, 0), length);
    pWorld.forceRegistry.Add(particle3, cable3);

    Cable* cable4 = new Cable(MyVector(0+distance, 160, 0), length);
    pWorld.forceRegistry.Add(particle4, cable4);

    Cable* cable5 = new Cable(MyVector(0+(distance*2), 160, 0), length);
    pWorld.forceRegistry.Add(particle5, cable5);
    
    std::vector<Cable*> cables = std::vector<Cable*>();

    // store them in a vector for ease of access later
    cables.push_back(cable1);
    cables.push_back(cable2);
    cables.push_back(cable3);
    cables.push_back(cable4);
    cables.push_back(cable5);

    RenderLine* line1 = new RenderLine(cable1->getAnchorPoint(), particle1->position, MyVector(1, 1, 1));
    RenderLine* line2 = new RenderLine(cable2->getAnchorPoint(), particle2->position, MyVector(1, 1, 1));
    RenderLine* line3 = new RenderLine(cable3->getAnchorPoint(), particle3->position, MyVector(1, 1, 1));
    RenderLine* line4 = new RenderLine(cable4->getAnchorPoint(), particle4->position, MyVector(1, 1, 1));
    RenderLine* line5 = new RenderLine(cable5->getAnchorPoint(), particle5->position, MyVector(1, 1, 1));

    std::vector<RenderLine*> lines = std::vector<RenderLine*>();

    // store them in a vector for ease of access later
    lines.push_back(line1);
    lines.push_back(line2);
    lines.push_back(line3);
    lines.push_back(line4);
    lines.push_back(line5);

    ///////////////////////////////////////////////////
  
    glfwSetKeyCallback(window, Key_CallBack); //calls function for updating x,y,z of the camera

    bool pause = false;
    auto lastSpawnTime = clock::now(); //timer for spawn
    int spawnedObjects = 0;  //detects alst object spawn time

    while (!glfwWindowShouldClose(window))
    {
        ortho->setCameraUse(orthoB);
        per->setCameraUse(perB);

        glm::vec3 pos;

        //sets the movement of the camera to rotate around the center with a radius of 400
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
            prev_time = clock::now(); //if played again right after pause , reset prev time to prevent 'jumping'
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
        }
        else {
            pause = true;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        
        int index = 0;

        for (RenderParticle* particle : vecRenderParticle) {
            // if switch to ortho camera, use its projection and view matrix
            if (ortho->getCameraUse()) {
                shader->setMat4("projection", 1, ortho->getProjectionMatrix());
                shader->setMat4("view", 1, ortho->getViewMatrix());
            }

            // switch if perspective camera is in use, use its projection and view matrix
            if (per->getCameraUse()) {
                
                shader->setMat4("projection", 1, per->getProjectionMatrix());
                shader->setMat4("view", 1, per->getViewMatrix());
            }

            // to maintain the white color of the lines
            glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
            shader->setGLfloat("color", 1 , &white[0]);

            //update and draw the line
            lines[index]->Update(cables[index]->getAnchorPoint(), particle->getParticle()->position);
            lines[index]->Draw(shader);

            // to maintain the color of each particle
            shader->setGLfloat("color", 1, particle->object->color);
            
            // call the particle's draw function
            particle->Draw();

            index++;
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