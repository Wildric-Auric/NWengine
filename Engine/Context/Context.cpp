#include "Context.h"
#include "Globals.h"
#include <iostream>

GLFWwindow* Context::window = nullptr;
int Context::WINDOW_WIDTH =  850;
int Context::WINDOW_HEIGHT = 640;

int Context::NATIVE_WIDTH = 850;
int Context::NATIVE_HEIGHT = 640;

int Context::dllFlag = NW_KEEP_DLL_RUNNING;

void sizeCallBack(GLFWwindow* window, int width, int height)
{
		Context::WINDOW_WIDTH = width;
		Context::WINDOW_HEIGHT = height;
}

void Context::SetViewPort(int x, int y, int sizeX, int sizeY) {
	glViewport(x, y, sizeX, sizeY);
}

void Context::SetFullscreen(bool state) {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
	if (!state) {
		glfwSetWindowMonitor(window, nullptr, 100, 100, Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT, vidmode->refreshRate);
		sizeCallBack(window, Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT);
		return;
	}
	glfwSetWindowMonitor(window, monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
	sizeCallBack(window, vidmode->width, vidmode->height);
}

GLFWwindow* Context::InitContext(int scrWidth, int scrHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(scrWidth, scrHeight, "NWengine", NULL, NULL);

	if (window == NULL)
	{
		const char* buffer = "";
		std::cout << "Failed to init glfw window, ERROR: " <<glfwGetError(&buffer) << " " << buffer;
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, sizeCallBack);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW";
		return nullptr;
	}
	glViewport(0, 0, Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT);
	return window;
}

void Context::EnableBlend(bool status) {
	if (status) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return;
	}

	glDisable(GL_BLEND);
	
}

void Context::EnableDepthTest(bool status) {
	if (status) {
		glEnable(GL_DEPTH_TEST);
		return;
	}
	glDisable(GL_DEPTH_TEST);
}

void Context::EnableWireframe(bool status) {
	if (status) {
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		return;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Context::Clear(float r, float g, float b, float a) { ///RGBA
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}