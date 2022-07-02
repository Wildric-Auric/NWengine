#include "Game.h"
#include "Globals.h"
#include <iostream>

void sizeCallBack(GLFWwindow* window, int width, int height)
{
		Globals::WINDOW_WIDTH = width;
		Globals::WINDOW_HEIGHT = height;
		//RENDERING_WIDTH = (int)(height * SCREENRATIO);
		//RENDERING_HEIGHT = height;
	    glViewport(0, 0, Globals::NATIVE_WIDTH, Globals::NATIVE_HEIGHT);
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
	glViewport(0, 0, Globals::NATIVE_WIDTH, Globals::NATIVE_HEIGHT);
	return window;
}

void EnableBlend(bool status) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void EnableWireframe(bool status) {
	if (status) {
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}