#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Airplane {
public:

    glm::vec3 position;
    glm::vec3 rotation; 
    glm::vec3 velocity;
    float speed;
    

    glm::vec3 bodyColor;

    Airplane(glm::vec3 startPosition = glm::vec3(0.0f, 10.0f, 0.0f));
    
    void update(float deltaTime);
    void draw(const Shader& shader);
    void rotate(float pitchDelta, float yawDelta, float rollDelta);
    void accelerate(float speedDelta);

    glm::vec3 getForwardVector() const;
    glm::vec3 getUpVector() const;
    glm::vec3 getRightVector() const;
    
private:

    unsigned int triangleVAO, triangleVBO;
    

    void setupTriangle();
    

    void drawBody(const Shader& shader);
    

    glm::mat4 createTransformMatrix(const glm::vec3& offset) const;
};

#endif 
