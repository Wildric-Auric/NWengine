#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include<fstream>
#include<string>
#include"Primitives.h"
#include "ShaderManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Consts
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
const float SCREENRATIO = ((float) SCREEN_WIDTH )/ ((float) SCREEN_HEIGHT);

int left, right, up, down = 0;
//Uniforms
GLfloat uTime = 0;
void sizeCallBack(GLFWwindow* window, int width, int height)
{
	float w = SCREENRATIO * SCREEN_HEIGHT;
	glViewport(0, 0, w, height);
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = height;

}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		left = true;
	}
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		right = true;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		up = true;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		down = true;
};
//The whole input solution is temporary;


GLFWwindow* InitContext()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_WIDTH, "TDS", NULL, NULL);
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

float pos = 0; //TODO: Delete this temp value
float posY = 0;
int main()
{

	GLFWwindow* window = InitContext();
	if (window == nullptr) return -1;

	
	Triangle* tr = new Triangle;
	Shader* defaultShader = new Shader();
	Quad* quad = new Quad();
	*defaultShader = Shader();
	*tr = Triangle();
	*quad = Quad(100.0, 100.0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 proj = glm::ortho(-((float)SCREEN_WIDTH),(float) SCREEN_WIDTH, -(float) SCREEN_HEIGHT, (float) SCREEN_HEIGHT);
	

	while (!glfwWindowShouldClose(window)) {
		uTime += 0.016;
		glClearColor(0.1f, 0.1f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		left = false;
		right = false;
		up = 0;
		down = 0;
		processInput(window);
		pos += (-left + right) * 2.0;
		posY += (-down + up) * 2.0;
		defaultShader->SetMat4x4("projectionMat", &proj[0][0]);
		defaultShader->SetUniform1f("uTime", uTime);
		defaultShader->SetVector2("uResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		defaultShader->SetVector2("uTranslation", pos, posY);
		//tr->Draw();
		quad->Draw();
		glUseProgram(defaultShader->shaderProgram);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}