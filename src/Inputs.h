#pragma once
#include "glfw3.h"
#include "Maths.h"

extern double mousePosX;
extern double mousePosY;
extern bool input_left, 
input_right, 
input_up, 
input_down,
input_d,
input_left_click
;
void processInput(GLFWwindow* window);
