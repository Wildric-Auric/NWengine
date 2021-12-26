#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//Put global variables here
extern double deltaTime;      //Read only in everyfile except mainloops

//Consts
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern const float SCREENRATIO;
extern const bool DEBUG_MODE;
//Uniforms
extern float uTime;
extern glm::mat4 projectionMatrix;
extern glm::mat4 viewMatrix;
