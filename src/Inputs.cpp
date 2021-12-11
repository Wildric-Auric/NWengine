#include "glfw3.h"
#include "Inputs.h"

bool input_left, input_right, input_up, input_down = 0;
void processInput(GLFWwindow* window)
{
	input_left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
	input_right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
	input_up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	input_down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;

};