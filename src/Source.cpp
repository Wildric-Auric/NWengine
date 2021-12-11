#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include<fstream>
#include<string>
#include"Primitives.h"
#include "ShaderManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Inputs.h"

//Consts
int SCREEN_WIDTH = 300;
int SCREEN_HEIGHT = 700;
const float SCREENRATIO = ((float) SCREEN_WIDTH )/((float) SCREEN_HEIGHT);
//Variable
float fps = 60;
int frameCount = 0;
double deltaTime = 0.016;
double currentTime;
double lastTime;

//Uniforms
GLfloat uTime = 0;
void sizeCallBack(GLFWwindow* window, int width, int height)
{
	float w = SCREENRATIO * SCREEN_HEIGHT;
	glViewport(0, 0, w, height);
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = height;

}

//The whole input solution is temporary;


GLFWwindow* InitContext()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TDS", NULL, NULL);
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

	glm::mat4 proj = glm::ortho(-((float)SCREEN_WIDTH)/2.0,(float) SCREEN_WIDTH/2.0, -(float) SCREEN_HEIGHT/2.0, (float) SCREEN_HEIGHT/2.0);
	
	lastTime = glfwGetTime();
	fps = 60.0;
	while (!glfwWindowShouldClose(window)) {
		uTime += 0.016;
		glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		pos += (-input_left + input_right)*300.0*deltaTime;
		posY += (-input_down + input_up)*300.0*deltaTime;
		defaultShader->SetMat4x4("projectionMat", &proj[0][0]);
		defaultShader->SetUniform1f("uTime", uTime);
		defaultShader->SetVector2("uResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		defaultShader->SetVector2("uTranslation", pos, posY);
		//tr->Draw();
		quad->Draw();
		glUseProgram(defaultShader->shaderProgram);
		


		glfwSwapBuffers(window);
		glfwPollEvents();

		//Calculate fps
		currentTime = glfwGetTime();
		deltaTime = currentTime- lastTime;
		lastTime = currentTime; //Well it's negligeable operation
		//frameCount++;
		//if (currentTime - lastTime >= 1.0) {
		//	lastTime = currentTime;
		//	fps = (float) frameCount;
		//	std::cout << currentTime - lastTime << std::endl; //TODO: Display this when you add text system
		//	frameCount = 0;
		//}
	}
	glfwTerminate();
}