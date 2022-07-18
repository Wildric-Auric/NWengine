#include "Inputs.h"
#include "Globals.h"
#include<iostream>
#include <cstring>

 bool Inputs::left, Inputs::right, Inputs::up, Inputs::down, Inputs::d,
 Inputs::n_1, Inputs::n_2, Inputs::n_3, Inputs::n_0, Inputs::n_4, Inputs::left_click,
 Inputs::space, Inputs::usingJoystick, Inputs::f2, Inputs::enter
 = 0;
double Inputs::mousePosX;
double Inputs::mousePosY;

float Inputs::joystickAxis[6] = {0.0f};

void Inputs::Process(GLFWwindow* window)
{
	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	mousePosY = Globals::WINDOW_HEIGHT - mousePosY;   //Down left as origin
	usingJoystick = glfwJoystickPresent(GLFW_JOYSTICK_1);
	const unsigned char* buttons = 0;
	if (usingJoystick) {
		int count;
		const float* local = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		int buttonCount;
		buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		memcpy(joystickAxis, local, count * sizeof(float)); //TODO::See if memcpy is the  Best Solution here
		for (int i = 0; i < count; i++) {
			if (abs(joystickAxis[i]) < 0.01) joystickAxis[i] = 0;
		}
	}

	left = ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS );
	right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
	up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
	d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	n_0 = glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS;
	n_1 = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	n_2 = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	n_3 = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
	n_4 = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
	space = ( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ) || ( usingJoystick&& GLFW_PRESS == buttons[1] ); 
	f2 = (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS);
	enter = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);


};