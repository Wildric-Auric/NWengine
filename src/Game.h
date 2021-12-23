#ifndef GAME_H
#define GAME_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"

GLFWwindow* InitContext(int scrWidth, int scrHeight);
void sizeCallBack(GLFWwindow* window, int width, int height);

#endif