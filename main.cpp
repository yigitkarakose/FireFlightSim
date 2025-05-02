#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "shader.h"
#include "camera.h"
#include "scene.h"
#include "airplane.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

glm::vec3 buildingColor = glm::vec3(0.8f, 0.8f, 0.8f); // Light grey
glm::vec3 asphaltColor = glm::vec3(0.2f, 0.2f, 0.2f);  // Dark grey
glm::vec3 beachColor = glm::vec3(0.76f, 0.7f, 0.5f);   // Sandy
glm::vec3 oceanColor = glm::vec3(0.0f, 0.3f, 0.7f);    // Deep blue
glm::vec3 forestGroundColor = glm::vec3(0.33f, 0.41f, 0.18f); // Forest green
glm::vec3 treeTrunkColor = glm::vec3(0.55f, 0.27f, 0.07f);    // Brown
glm::vec3 treeLeavesColor = glm::vec3(0.1f, 0.5f, 0.1f);      // Green
glm::vec3 fireBaseColor = glm::vec3(1.0f, 0.0f, 0.0f);     // Red (base of fire)
glm::vec3 fireTopColor = glm::vec3(1.0f, 1.0f, 0.0f);      // Yellow (top of fire)

glm::vec3 lightDir = glm::normalize(glm::vec3(0.3f, -1.0f, 0.3f));
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // moonlight

Camera camera(glm::vec3(-150.0f, 10.0f, 0.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool accelerating = false;
bool decelerating = false;
bool turningLeft = false;
bool turningRight = false;
bool pitchingUp = false;
bool pitchingDown = false;
bool rolling = false;
bool unrolling = false;
bool droppingCube = false;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    Airplane* airplane = getAirplane();
    if (!airplane) return;
    
    accelerating = decelerating = turningLeft = turningRight = false;
    pitchingUp = pitchingDown = rolling = unrolling = false;
    droppingCube = false; 
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pitchingDown = true; // Pitch down (forward)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pitchingUp = true;   // Pitch up (backward)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        turningLeft = true;  // Yaw left
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        turningRight = true; // Yaw right
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        rolling = true;      // Roll counter-clockwise
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        unrolling = true;    // Roll clockwise
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        accelerating = true; // Increase speed
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        decelerating = true; // Decrease speed
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        droppingCube = true; // Drop a blue cube
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Firefighting Plane Sim", NULL, NULL);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD initialization failed\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    std::cout << "Loading shaders from: shaders/basic.vs and shaders/basic.fs" << std::endl;
    Shader shader("basic.vs", "basic.fs");

    setupScene();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.02f, 0.02f, 0.05f, 1.0f); // dark blue background for night
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 300.0f
        );
        
        Airplane* airplane = getAirplane();
        if (airplane) {
            if (accelerating)
                airplane->accelerate(deltaTime * 10.0f);
            if (decelerating)
                airplane->accelerate(-deltaTime * 10.0f);
            
            float rotationSpeed = deltaTime * 1.0f;
            if (pitchingUp)
                airplane->rotate(rotationSpeed, 0.0f, 0.0f);
            if (pitchingDown)
                airplane->rotate(-rotationSpeed, 0.0f, 0.0f);
            if (turningLeft)
                airplane->rotate(0.0f, rotationSpeed, 0.0f);
            if (turningRight)
                airplane->rotate(0.0f, -rotationSpeed, 0.0f);
            if (rolling)
                airplane->rotate(0.0f, 0.0f, rotationSpeed);
            if (unrolling)
                airplane->rotate(0.0f, 0.0f, -rotationSpeed);
            if (droppingCube)
                spawnBlueCube(); 
                
            airplane->update(deltaTime);
            
            camera.SetTarget(airplane->position, airplane->getForwardVector());
        }
        
        glm::mat4 view = camera.GetViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("lightDir", lightDir);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("viewPos", camera.Position);
        shader.setFloat("shininess", 20.0f);
        shader.setFloat("specularStrength", 0.8f);
        shader.setVec3("fillDir", glm::vec3(-lightDir.x, -lightDir.y, -lightDir.z));
        shader.setVec3("fillColor", glm::vec3(0.3f, 0.3f, 0.35f)); // hafif mavi-gri


        drawScene(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
