#pragma once
#include "glfw3.h"
#include "Maths.h"

#define INPUT_LEFT_X 0
#define INPUT_LEFT_Y 1
#define INPUT_RIGHT_X 3
#define INPUT_RIGHT_Y 4


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
input_space,

usingJoystick
;

extern float joystickAxis[6];
void processInput(GLFWwindow* window);