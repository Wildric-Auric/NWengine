#include "Game.h"
#include "Globals.h"
#include <iostream>

void sizeCallBack(GLFWwindow* window, int width, int height)
{
		WINDOW_WIDTH = width;
		WINDOW_HEIGHT = height;
		RENDERING_WIDTH = (int)(height * SCREENRATIO);
		RENDERING_HEIGHT = height;
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

GLFWwindow* InitContext(int scrWidth, int scrHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(scrWidth, scrHeight, "NWengine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to init glfw window";
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, sizeCallBack);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW";
		return 0;
	}
	return window;
}