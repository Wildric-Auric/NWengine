#pragma once
#include "Maths.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef Vector2<int> iVec2;
typedef Vector2<float> fVec2;
typedef Vector3<float> fVec3;
typedef Vector3<int> iVec3;
typedef Vector2<double> dVec2;
typedef Vector3<double> dVec3;

typedef Matrix<2, 2, int> iMat2;
//Put global variables here
extern double deltaTime;      //Read only in everyfile except mainloops

//Consts
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int RENDERING_WIDTH;
extern int RENDERING_HEIGHT;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern const float SCREENRATIO;
extern const bool DEBUG_MODE;
//Uniforms
extern float uTime;
extern glm::mat4 projectionMatrix;
extern glm::mat4 viewMatrix;
//Variables
extern bool isMouseOnGui;
extern iVec2 grav;

