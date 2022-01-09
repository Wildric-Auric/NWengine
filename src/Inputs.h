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
input_left_click,
input_0,
input_1,
input_2,
input_3,
input_4,
input_space
;
void processInput(GLFWwindow* window);
