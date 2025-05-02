#include "airplane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm_includes.h"  // Added to define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

Airplane::Airplane(glm::vec3 startPosition) 
    : position(startPosition), 
      rotation(0.0f, 0.0f, 0.0f),
      velocity(0.0f, 0.0f, 0.0f),
      speed(0.0f),
      bodyColor(0.0f, 0.0f, 0.0f)      // Black color for the triangle
{
    setupTriangle();
}

void Airplane::setupTriangle() {
    // Create a flat triangle in the x-z plane
    float vertices[] = {
        // positions          // normals
        // Triangle vertices forming a delta-wing shape
        -3.0f, -0.2f,  3.0f,  0.0f,  1.0f,  0.0f,  // Left back corner
         3.0f, -0.2f,  0.0f,  0.0f,  1.0f,  0.0f,  // Front point (nose)
        -3.0f, -0.2f, -3.0f,  0.0f,  1.0f,  0.0f,  // Right back corner
        
        // Bottom face triangle
        -3.0f, -0.4f,  3.0f,  0.0f, -1.0f,  0.0f,
         3.0f, -0.4f,  0.0f,  0.0f, -1.0f,  0.0f,
        -3.0f, -0.4f, -3.0f,  0.0f, -1.0f,  0.0f,
        
        // Left face
        -3.0f, -0.2f,  3.0f, -0.5f,  0.0f,  0.5f,
        -3.0f, -0.4f,  3.0f, -0.5f,  0.0f,  0.5f,
         3.0f, -0.4f,  0.0f,  0.5f,  0.0f,  0.0f,
         
         3.0f, -0.4f,  0.0f,  0.5f,  0.0f,  0.0f,
         3.0f, -0.2f,  0.0f,  0.5f,  0.0f,  0.0f,
        -3.0f, -0.2f,  3.0f, -0.5f,  0.0f,  0.5f,
        
        // Right face
        -3.0f, -0.2f, -3.0f, -0.5f,  0.0f, -0.5f,
        -3.0f, -0.4f, -3.0f, -0.5f,  0.0f, -0.5f,
         3.0f, -0.4f,  0.0f,  0.5f,  0.0f,  0.0f,
         
         3.0f, -0.4f,  0.0f,  0.5f,  0.0f,  0.0f,
         3.0f, -0.2f,  0.0f,  0.5f,  0.0f,  0.0f,
        -3.0f, -0.2f, -3.0f, -0.5f,  0.0f, -0.5f,
        
        // Back face
        -3.0f, -0.2f,  3.0f, -1.0f,  0.0f,  0.0f,
        -3.0f, -0.4f,  3.0f, -1.0f,  0.0f,  0.0f,
        -3.0f, -0.4f, -3.0f, -1.0f,  0.0f,  0.0f,
        
        -3.0f, -0.4f, -3.0f, -1.0f,  0.0f,  0.0f,
        -3.0f, -0.2f, -3.0f, -1.0f,  0.0f,  0.0f,
        -3.0f, -0.2f,  3.0f, -1.0f,  0.0f,  0.0f,
    };

    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Airplane::update(float deltaTime) {
    // Update velocity based on current orientation
    glm::vec3 forward = getForwardVector();
    velocity = forward * speed;
    
    // Update position based on velocity
    position += velocity * deltaTime;
}

void Airplane::draw(const Shader& shader) {
    // Draw the triangle airplane
    drawBody(shader);
}

glm::mat4 Airplane::createTransformMatrix(const glm::vec3& offset) const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + offset);
    
    // Apply rotation
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    
    return model;
}

void Airplane::drawBody(const Shader& shader) {
    shader.setVec3("objectColor", bodyColor);
    
    glm::mat4 model = createTransformMatrix(glm::vec3(0.0f, 0.0f, 0.0f));
    
    shader.setMat4("model", model);
    
    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 24);
}

void Airplane::rotate(float pitchDelta, float yawDelta, float rollDelta) {
    rotation.x += pitchDelta;
    rotation.y += yawDelta;
    rotation.z += rollDelta;
}

void Airplane::accelerate(float speedDelta) {
    speed += speedDelta;
    
    // Clamp speed to reasonable values
    if (speed < 0.0f) speed = 0.0f;
    if (speed > 100.0f) speed = 100.0f;
}

glm::vec3 Airplane::getForwardVector() const {
    // Calculate forward direction based on current rotation
    glm::vec3 forward(1.0f, 0.0f, 0.0f); // Default forward is along positive X
    
    // Apply yaw (y-axis rotation)
    forward = glm::rotateY(forward, rotation.y);
    
    // Apply pitch (x-axis rotation)
    forward = glm::rotateX(forward, rotation.x);
    
    // Apply roll (z-axis rotation, doesn't affect forward vector)
    
    return glm::normalize(forward);
}

glm::vec3 Airplane::getUpVector() const {
    // Calculate up direction based on current rotation
    glm::vec3 up(0.0f, 1.0f, 0.0f); // Default up is along positive Y
    
    // Apply yaw (y-axis rotation)
    up = glm::rotateY(up, rotation.y);
    
    // Apply pitch (x-axis rotation)
    up = glm::rotateX(up, rotation.x);
    
    // Apply roll (z-axis rotation)
    up = glm::rotateZ(up, rotation.z);
    
    return glm::normalize(up);
}

glm::vec3 Airplane::getRightVector() const {
    // Right vector is cross product of forward and up vectors
    return glm::normalize(glm::cross(getForwardVector(), getUpVector()));
}
