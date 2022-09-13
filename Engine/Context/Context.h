#pragma once
#include "GL/glew.h"
#include "glfw3.h"


class Context {
public:
	static GLFWwindow* window;  //In the futute I will create class window which may be opengl, vulkan, or directx window
	static GLFWwindow* InitContext(int scrWidth, int scrHeight);
	static void EnableBlend(bool status = 1);
	static void EnableWireframe(bool status = 1);
	static void EnableDepthTest(bool status = 1);
	static void Clear(float r = .0f, float g = .0f, float b = 0.0f, float a = 1.0f);
	static void SetViewPort(int x, int y, int sizeX, int sizeY);

	static int  NATIVE_WIDTH;
	static int NATIVE_HEIGHT;

	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
};

