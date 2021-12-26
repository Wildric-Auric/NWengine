#include "Inputs.h"

bool input_left, input_right, input_up, input_down,input_d = 0;
double mousePosX;
double mousePosY;
void processInput(GLFWwindow* window)
{
	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	input_left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
	input_right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
	input_up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	input_down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
	input_d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
};