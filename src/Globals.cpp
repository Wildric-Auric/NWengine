#include "Globals.h"

double deltaTime = 0.016;        //Read only in everyfile except mainloops

//Consts
int SCREEN_WIDTH = 850;
int SCREEN_HEIGHT = 640;
const float SCREENRATIO = ((float)SCREEN_WIDTH) / ((float)SCREEN_HEIGHT);

//Uniforms
float uTime = 0;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix(1.0);
