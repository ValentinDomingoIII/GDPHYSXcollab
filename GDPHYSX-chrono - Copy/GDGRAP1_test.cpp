#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Physics/MyVector.h"
#include "Physics/P6Particle.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std::chrono_literals;
//THis is going tobe time between frames
constexpr std::chrono::nanoseconds timestep(16ms);
float x_mod = 0;
float y_mod = 0;
float z_mod = 0.f;
float theta_mod = 0;
float scale_mod = 1;


void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (key == GLFW_KEY_D) {
        //move bunny to right
        x_mod += 0.01f;
    }

    if (key == GLFW_KEY_A) {
        //move bunny to left
        x_mod += -0.01f;
    }

    if (key == GLFW_KEY_S) {
        //move bunny to down
        y_mod += -0.01f;
    }

    if (key == GLFW_KEY_W) {
        //move bunny to down
        y_mod += 0.01f;
    }
    if (key == GLFW_KEY_Y) {
        theta_mod += 2.0f;
    }
    if (key == GLFW_KEY_X) {
        //move bunny to down
        scale_mod += 0.01f;
    }

    if (key == GLFW_KEY_J) {
        //move bunny to down
        z_mod += 0.1f;
    }
    if (key == GLFW_KEY_K) {
        //move bunny to down
        z_mod -= 0.1f;
    }



}

class Object {
public:
    //create VAO / VBO Variables
    GLuint VAO, VBO, EBO;
    std::string path = "3D/bola.obj";
    std::vector<tinyobj::shape_t>shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices;

    glm::vec3 pos = glm::vec3(0, 0, 0);

    //Initialize the VAO and VBO
    Object() {
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

        //Currently Editing VAO = null
        glBindVertexArray(VAO);
        //Currently editing VBO = null
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //Currently editing VBO = VBO

        glBufferData(GL_ARRAY_BUFFER,
            sizeof(GL_FLOAT) * attributes.vertices.size(),//bytes
            attributes.vertices.data(),// array
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            0,
            3, // x y z
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            (void*)0
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLuint) * mesh_indices.size(),
            mesh_indices.data(),
            GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        //Currently editing VBO = VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //Currently editing VBO = null 

        //Currently editing VAO = VAO
        glBindVertexArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

int main(void)
{
    float x = 0, y = 0, z = -2.f, scale_x = 10, scale_y = 10, scale_z = 10, theta = 90, axis_x = 0, axis_y = 1, axis_z = 0;
    float height = 600.0f;
    float width = 600.0f;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Bumanglag, Domingo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    //glViewport(0, 0, 600, 600);

    glfwSetKeyCallback(window, Key_Callback);
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();
    //Create a vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);


    //error checker
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
        std::cout << &errorLog[0];
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        //glDeleteShader(vertexShader); // Don't leak the shader.
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);


    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);
        std::cout << &errorLog[0];
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
       // glDeleteShader(fragShader); // Don't leak the shader.
    }

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);
    GLfloat vertices[]{
        //x  y   z
        0.f,0.5f,0.f,
        -0.5f, -0.5f, 0.f,
        0.5f,-0.5f ,0.f
    };

    GLuint indices[]{
        0,1,2
    };


    std::vector<Object*> objects;

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    //orthographiuc usage
    glm::mat4 projectionMatrix = glm::ortho(
        -width / 2.f, //left
        width / 2.f, //right
        -height / 2.f, //bot
        height / 2.f, //top
        -100.f, //z near
        100.f);  //z fa

    Physics::MyVector sample(0, 0, 0);
    Physics::MyVector toAdd(0, 0, 0);

    Physics::P6Particle particle = Physics::P6Particle();
    particle.velocity = Physics::MyVector(100, 0, 0);
    particle.acceleration = Physics::MyVector(-30, 0, 0);

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds  curr_ns(0);

    Object obj;

    particle.position = sample;

    while (!glfwWindowShouldClose(window))
    {
        //Get the current time
        curr_time = clock::now();
        //check the duration in between the last iteration
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        //set prev with the current for the next iteration
        prev_time = curr_time;

        curr_ns += dur;
        if (curr_ns >= timestep)
        {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            std::cout << "MS:" << (float)ms.count() << "\n";
            //reset
            curr_ns -= timestep;

            //convert ms to seconds
            particle.Update((float)ms.count() / 1000);
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);  

        glClear(GL_COLOR_BUFFER_BIT);

        obj.pos = ((glm::vec3)particle.position);  // Update position based on user input

        theta = theta_mod;

        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            obj.pos
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix));

        glUseProgram(shaderProg);

        glBindVertexArray(obj.VAO);
        glDrawElements(GL_TRIANGLES, obj.mesh_indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwPollEvents();
    }

    for (auto obj : objects)
    {
        glDeleteVertexArrays(1, &obj->VAO);
        glDeleteBuffers(1, &obj->VBO);
        glDeleteBuffers(1, &obj->EBO);
    }

    glfwTerminate();
    return 0;
}