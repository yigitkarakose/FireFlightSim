#include "scene.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <ctime> // Added missing include for time functions

// General
unsigned int cubeVAO, cubeVBO, cubeEBO;
std::vector<glm::vec3> treePositions;
std::vector<float> treeHeights;
std::vector<glm::vec3> buildingPositions;
std::vector<float> buildingHeights;
std::vector<glm::vec2> oceanTiles;
std::vector<glm::vec2> beachTiles;

// Airplane
Airplane* airplane = nullptr;

// Fire prism variables (removed duplicate declaration)
unsigned int fireVAO, fireVBO, fireEBO;
std::vector<glm::vec3> firePositions;
const int NUM_FIRE_POSITIONS = 25; // Number of fire spots

// Blue cube variables
std::vector<glm::vec3> blueCubePositions; // Store positions of multiple cubes
glm::vec3 blueCubeColor = glm::vec3(0.0f, 0.3f, 1.0f); // Blue color

// Use color variables defined in Main.cpp
extern glm::vec3 buildingColor;
extern glm::vec3 asphaltColor;
extern glm::vec3 beachColor;
extern glm::vec3 oceanColor;
extern glm::vec3 forestGroundColor;
extern glm::vec3 treeTrunkColor;
extern glm::vec3 treeLeavesColor;

float cubeVertices[] = {
    // pos              // normal           // texcoord removed
    // back face
    -0.5f,-0.5f,-0.5f, 0,0,-1,
     0.5f,-0.5f,-0.5f, 0,0,-1,
     0.5f, 0.5f,-0.5f, 0,0,-1,
    -0.5f, 0.5f,-0.5f, 0,0,-1,

    // front face
    -0.5f,-0.5f, 0.5f, 0,0,1,
     0.5f,-0.5f, 0.5f, 0,0,1,
     0.5f, 0.5f, 0.5f, 0,0,1,
    -0.5f, 0.5f, 0.5f, 0,0,1,

    // left face
    -0.5f,-0.5f,-0.5f, -1,0,0,
    -0.5f, 0.5f,-0.5f, -1,0,0,
    -0.5f, 0.5f, 0.5f, -1,0,0,
    -0.5f,-0.5f, 0.5f, -1,0,0,

    // right face
     0.5f,-0.5f,-0.5f, 1,0,0,
     0.5f, 0.5f,-0.5f, 1,0,0,
     0.5f, 0.5f, 0.5f, 1,0,0,
     0.5f,-0.5f, 0.5f, 1,0,0,

     // bottom face
     -0.5f,-0.5f,-0.5f, 0,-1,0,
      0.5f,-0.5f,-0.5f, 0,-1,0,
      0.5f,-0.5f, 0.5f, 0,-1,0,
     -0.5f,-0.5f, 0.5f, 0,-1,0,

     // top face
     -0.5f, 0.5f,-0.5f, 0,1,0,
      0.5f, 0.5f,-0.5f, 0,1,0,
      0.5f, 0.5f, 0.5f, 0,1,0,
     -0.5f, 0.5f, 0.5f, 0,1,0
};

unsigned int cubeIndices[] = {
    0,1,2, 2,3,0,  4,5,6, 6,7,4,  4,5,1, 1,0,4,
    7,6,2, 2,3,7,  4,0,3, 3,7,4,  5,1,2, 2,6,5
};

void setupCube() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Position attribute - make sure stride and offset are correct
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute - make sure stride and offset are correct
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO to prevent accidental modifications
    glBindVertexArray(0);
}

void setupFirePrisms() {
    // Vertices for a triangular prism
    float fireVertices[] = {
        // Position          // Normal
        // Bottom triangle
        -0.25f, 0.0f, -0.25f,   0.0f, -1.0f, 0.0f, // Bottom left
         0.25f, 0.0f, -0.25f,   0.0f, -1.0f, 0.0f, // Bottom right
         0.0f,  0.0f,  0.25f,   0.0f, -1.0f, 0.0f, // Bottom front
        
        // Top triangle
        -0.25f, 1.0f, -0.25f,   0.0f, 1.0f, 0.0f, // Top left
         0.25f, 1.0f, -0.25f,   0.0f, 1.0f, 0.0f, // Top right
         0.0f,  1.0f,  0.25f,   0.0f, 1.0f, 0.0f, // Top front
        
        // Side faces (normals are approximate for simplicity)
        // Left face
        -0.25f, 0.0f, -0.25f,  -0.8f, 0.0f, -0.6f,
        -0.25f, 1.0f, -0.25f,  -0.8f, 0.0f, -0.6f,
         0.0f,  0.0f,  0.25f,  -0.8f, 0.0f, -0.6f,
         0.0f,  1.0f,  0.25f,  -0.8f, 0.0f, -0.6f,
        
        // Right face
         0.25f, 0.0f, -0.25f,   0.8f, 0.0f, -0.6f,
         0.25f, 1.0f, -0.25f,   0.8f, 0.0f, -0.6f,
         0.0f,  0.0f,  0.25f,   0.8f, 0.0f, -0.6f,
         0.0f,  1.0f,  0.25f,   0.8f, 0.0f, -0.6f,
        
        // Back face
        -0.25f, 0.0f, -0.25f,   0.0f, 0.0f, -1.0f,
        -0.25f, 1.0f, -0.25f,   0.0f, 0.0f, -1.0f,
         0.25f, 0.0f, -0.25f,   0.0f, 0.0f, -1.0f,
         0.25f, 1.0f, -0.25f,   0.0f, 0.0f, -1.0f
    };

    unsigned int fireIndices[] = {
        // Bottom triangle
        0, 1, 2,
        
        // Top triangle
        3, 5, 4,
        
        // Side faces
        // Left face
        6, 7, 8,
        7, 9, 8,
        
        // Right face
        10, 12, 11, 
        11, 12, 13,
        
        // Back face
        14, 15, 16,
        15, 17, 16
    };

    // Generate random fire positions in the forest area
    srand((unsigned)time(0) + 1000); // Different seed from trees
    for (int i = 0; i < NUM_FIRE_POSITIONS; ++i) {
        // Position fires in the forest area (x: 60-140, z: -78 to 78)
        float x = 70.0f + rand() % 60;
        float z = -70.0f + rand() % 140;
        firePositions.push_back(glm::vec3(x, 0.0f, z));
    }

    // Create and configure the VAO, VBO, EBO for fire prisms
    glGenVertexArrays(1, &fireVAO);
    glGenBuffers(1, &fireVBO);
    glGenBuffers(1, &fireEBO);

    glBindVertexArray(fireVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fireVertices), fireVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fireEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fireIndices), fireIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void spawnBlueCube() {
    // Get the airplane position and spawn the cube from there
    if (airplane) {
        blueCubePositions.push_back(airplane->position);
    }
}

void generatePositions() {
    srand((unsigned)time(0));
    // sadece ağaç ve binaları üret
    for (int i = 0; i < 500; ++i) {
        float x = 62.0f + rand() % 76, z = -78.0f + rand() % 156;
        float h = 2.0f + (rand() % 10) / 10.0f;
        treePositions.emplace_back(x, 0, z);
        treeHeights.push_back(h);
    }
    for (int i = 0; i < 100; ++i) {
        float x = -98.0f + rand() % 156, z = -78.0f + rand() % 156;
        float h = 7.0f + rand() % 20;
        buildingPositions.emplace_back(x, 0, z);
        buildingHeights.push_back(h);
    }
}

void setupScene() {
    setupCube();
    setupFirePrisms();
    generatePositions();
    
    // Create airplane at a starting position
    airplane = new Airplane(glm::vec3(-120.0f, 30.0f, -50.0f));
}

// Moved drawFirePrisms implementation to before it's used in drawScene
void drawFirePrisms(const Shader& shader) {
    glBindVertexArray(fireVAO);
    
    // Set fire prism flag
    shader.setBool("isFirePrism", true);
    
    // Draw each fire prism with height-based color gradient
    for (size_t i = 0; i < firePositions.size(); ++i) {
        glm::vec3 pos = firePositions[i];
        
        // Make fire heights slightly different for variety
        float baseHeight = 0.5f + ((float)(rand() % 30) / 100.0f); // 0.5 to 0.8
        
        // Draw 3-5 prisms for each fire with decreasing size
        for (int j = 0; j < 3 + rand() % 3; ++j) {
            float scale = 1.0f - (j * 0.2f); // Each layer is smaller
            float height = baseHeight - (j * 0.1f); // Each layer is shorter
            
            // Small random offset for each prism in the same fire
            float offsetX = ((float)(rand() % 20 - 10) / 100.0f); // -0.1 to 0.1
            float offsetZ = ((float)(rand() % 20 - 10) / 100.0f); // -0.1 to 0.1
            
            glm::mat4 model = glm::translate(glm::mat4(1.0f), 
                              pos + glm::vec3(offsetX, 0.0f, offsetZ));
            model = glm::scale(model, glm::vec3(scale * 0.8f, height, scale * 0.8f));
            shader.setMat4("model", model);
            
            // Set the height for color gradient (bottom parts are red, top parts are yellow)
            shader.setFloat("fireHeight", (float)j / 3.0f);
            
            // Draw the triangular prism
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        }
    }
    
    // Reset fire prism flag
    shader.setBool("isFirePrism", false);
    glBindVertexArray(0);
}

void updateAndDrawBlueCubes(const Shader& shader) {
    // Bind the cube VAO since we're using the same geometry
    glBindVertexArray(cubeVAO);
    
    // Set the blue cube color
    shader.setVec3("objectColor", blueCubeColor);
    
    // Update positions and draw each cube
    for (size_t i = 0; i < blueCubePositions.size(); ++i) {
        // Move downward on Y axis
        blueCubePositions[i].y -= 0.1f; // Simple movement, no physics
        
        // Draw the cube
        glm::mat4 model = glm::translate(glm::mat4(1.0f), blueCubePositions[i]);
        model = glm::scale(model, glm::vec3(0.5f)); // Small cube
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);
}

void drawScene(const Shader& shader) {
    glBindVertexArray(cubeVAO);

    // ─── Ground Bölümleri ───
    // 1) Ocean (-200 → -120 arası, genişlik 80, derinlik 160)
    shader.setVec3("objectColor", oceanColor);
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-160.0f, -0.51f, 0.0f));
        model = glm::scale(model, glm::vec3(80.0f, 0.1f, 160.0f));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // 2) Beach (-120 → -100 arası, genişlik 20)
    shader.setVec3("objectColor", beachColor);
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f, -0.51f, 0.0f));
        model = glm::scale(model, glm::vec3(20.0f, 0.1f, 160.0f));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // 3) Asphalt (-100 → 60 arası, genişlik 160)
    shader.setVec3("objectColor", asphaltColor);
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, -0.51f, 0.0f));
        model = glm::scale(model, glm::vec3(160.0f, 0.1f, 160.0f));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // 4) Forest (60 → 140 arası, genişlik 80)
    shader.setVec3("objectColor", forestGroundColor);
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, -0.51f, 0.0f));
        model = glm::scale(model, glm::vec3(80.0f, 0.1f, 160.0f));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // ─── Ağaç Gövdesi ───
    shader.setVec3("objectColor", treeTrunkColor);
    for (size_t i = 0; i < treePositions.size(); ++i) {
        glm::vec3 p = treePositions[i];
        float h = treeHeights[i];
        glm::mat4 M = glm::translate(glm::mat4(1.0f), p + glm::vec3(0.0f, h / 2.0f, 0.0f));
        M = glm::scale(M, glm::vec3(0.8f, h, 0.8f));
        shader.setMat4("model", M);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // ─── Ağaç Yaprakları ───
    shader.setVec3("objectColor", treeLeavesColor);
    for (size_t i = 0; i < treePositions.size(); ++i) {
        glm::vec3 p = treePositions[i];
        float h = treeHeights[i];
        glm::mat4 M = glm::translate(glm::mat4(1.0f), p + glm::vec3(0.0f, h + 0.3f, 0.0f));
        M = glm::scale(M, glm::vec3(2.0f));
        shader.setMat4("model", M);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // ─── Binalar ───
    shader.setVec3("objectColor", buildingColor);
    for (size_t i = 0; i < buildingPositions.size(); ++i) {
        glm::vec3 p = buildingPositions[i];
        float h = buildingHeights[i];
        glm::mat4 M = glm::translate(glm::mat4(1.0f), p + glm::vec3(0.0f, h / 2.0f, 0.0f));
        M = glm::scale(M, glm::vec3(4.0f, h, 4.0f));
        shader.setMat4("model", M);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    
    // Draw fire prisms after all other scene elements
    drawFirePrisms(shader);
    
    // Draw the airplane
    if (airplane) {
        airplane->draw(shader);
    }
    
    // Update and draw blue cubes
    updateAndDrawBlueCubes(shader);
}

Airplane* getAirplane() {
    return airplane;
}

