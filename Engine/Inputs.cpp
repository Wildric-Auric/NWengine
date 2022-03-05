#include "Inputs.h"
#include "Globals.h"
#include<iostream>

bool input_left, input_right, input_up, input_down, input_d,
input_1, input_2, input_3, input_0, input_4, input_left_click,
input_space, usingJoyStick, input_f2, input_enter
= 0;
double mousePosX;
double mousePosY;

float joystickAxis[6];

void processInput(GLFWwindow* window)
{
	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	mousePosY = WINDOW_HEIGHT - mousePosY;   //Down left as origin
	usingJoyStick = glfwJoystickPresent(GLFW_JOYSTICK_1);
	const unsigned char* buttons = 0;
	if (usingJoyStick) {
		int count;
		const float* local = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		int buttonCount;
		buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		memcpy(joystickAxis, local, count * sizeof(float)); //TODO::See if memcpy is the  Best Solution here
		for (int i = 0; i < count; i++) {
			if (abs(joystickAxis[i]) < 0.01) joystickAxis[i] = 0;
		}
	}

	input_left = ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS );
	input_right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
	input_up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	input_down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
	input_d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	input_left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	input_0 = glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS;
	input_1 = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	input_2 = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	input_3 = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
	input_4 = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
	input_space = ( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ) || ( usingJoyStick&& GLFW_PRESS == buttons[1] ); 
	input_f2 = (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS);
	input_enter = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);


};