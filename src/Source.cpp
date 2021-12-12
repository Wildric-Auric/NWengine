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
#include "Maths.h"
#include "Game.h"
//Consts
int SCREEN_WIDTH = 800;
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

glm::mat4 proj;
glm::mat4 view(1.0);


int main()
{

	GLFWwindow* window = InitContext(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == nullptr) return -1;

	
	Triangle* tr = new Triangle;
	Shader* defaultShader = new Shader();
	Quad* quad = new Quad();
	*defaultShader = Shader();
	*tr = Triangle();
	*quad = Quad(Vector2(0.0f,0.0f), 100.0, 100.0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	proj = glm::ortho(-((float)SCREEN_WIDTH)/2.0,(float) SCREEN_WIDTH/2.0, -(float) SCREEN_HEIGHT/2.0, (float) SCREEN_HEIGHT/2.0);
	
	lastTime = glfwGetTime();
	fps = 60.0;
	while (!glfwWindowShouldClose(window)) {
		uTime += deltaTime;
		glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		quad->position.x += (-input_left + input_right)*300.0*deltaTime;
		quad->position.y += (-input_down + input_up)*300.0*deltaTime;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(quad->position.x, quad->position.y, 0.0f));
		defaultShader->SetMat4x4("uMvp", &(proj*view*model)[0][0]);

		defaultShader->SetUniform1f("uTime", uTime);
		defaultShader->SetVector2("uResolution", SCREEN_WIDTH, SCREEN_HEIGHT);

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