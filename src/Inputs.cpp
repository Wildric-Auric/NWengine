#include "Inputs.h"
#include "Globals.h"
bool input_left, input_right, input_up, input_down, input_d,
input_1, input_2, input_3, input_0, input_4, input_left_click,
input_space
= 0;
double mousePosX;
double mousePosY;
void processInput(GLFWwindow* window)
{
	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	mousePosY = WINDOW_HEIGHT - mousePosY;
	input_left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
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
	input_space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

};