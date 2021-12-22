#pragma once
#include "GLFW/glfw3.h"
#include "Maths.h"

extern double mousePosX;
extern double mousePosY;
extern bool input_left, 
input_right, 
input_up, 
input_down;
void processInput(GLFWwindow* window);