#ifndef SCENE_H
#define SCENE_H

#include "shader.h"
#include "airplane.h"
#include <glm/glm.hpp>
#include <vector>

void setupScene();
void drawScene(const Shader& shader);
Airplane* getAirplane(); 


void setupFirePrisms();
void drawFirePrisms(const Shader& shader);

void spawnBlueCube();
void updateAndDrawBlueCubes(const Shader& shader);

#endif 