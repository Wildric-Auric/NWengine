#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Primitives.h"
#include "ShaderManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Inputs.h"
#include "Maths.h"
#include "Game.h"
#include "Texture.h"
//#include "Text.h"
#include "RessourcesLoader.h"
#include "Time.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <irrKlang.h>
//Consts
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 700;
const float SCREENRATIO = ((float) SCREEN_WIDTH )/((float) SCREEN_HEIGHT);
//Variable
float fps = 60;
int frameCount = 0;

double currentTime;
double lastTime;

//Uniforms
GLfloat uTime = 0;

glm::mat4 proj;
glm::mat4 view(1.0);
using namespace irrklang;
int main()
{ 
    //Init GLFW context 
	GLFWwindow* window = InitContext(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == nullptr) return -1;

	//Create ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//init irrKlang
	ISoundEngine* SoundEngine = createIrrKlangDevice();

	//Load ressources
	loadImages();
	Texture tex = Texture(TEX1.width, TEX1.height, TEX1.tex, 1, 0);

	//GrabPass test
	GLubyte* behindPixels = new GLubyte[4 * SCREEN_WIDTH * SCREEN_HEIGHT];


	Shader* defaultShader		= new Shader();
	Shader* lightSurfaceShader	= new Shader();
	Shader* grabPassShader		= new Shader();
	//Shader* textShader			= new Shader();

	Quad* quad			= new Quad();
	Quad* lightSurface	= new Quad();
	Quad* grabPass		= new Quad();

	//Text* text	= new Text();
	//int	init	= text->initfreetype("fonts/rockstar.otf");

	*defaultShader		= Shader("Shaders/Shader1.shader");
	*lightSurfaceShader	= Shader("Shaders/LightSurface.shader");
	*grabPassShader		= Shader("Shaders/GrabPass.shader");
	//*textShader			= Shader("Shaders/Text.shader");

	*quad			= Quad(Vector2(0.0f,0.0f), 300.0F, 300.0F);
	*lightSurface	= Quad(Vector2(0.0f, 0.0f), (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	*grabPass		= Quad(Vector2(0.0f, 0.0f), (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	proj = glm::ortho(-((float)SCREEN_WIDTH)/2.0f,(float) SCREEN_WIDTH/2.0f, -(float) SCREEN_HEIGHT/2.0f, (float) SCREEN_HEIGHT/2.0f);
	
	lastTime = glfwGetTime();
	fps = 60.0;
	Texture grabTex = Texture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels);

	ImColor bgColor = ImColor(82, 75, 108);

	irrklang::ISound* sd = SoundEngine->play2D("Ressources/Sounds/Mystery.mp3", true, false, true);
	if (sd) sd->setVolume(0.3);

	while (!glfwWindowShouldClose(window)){
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::ColorEdit3("Background Color", (float*)&bgColor);
		ImGui::End();
		//----
		uTime += deltaTime;
		glClearColor(bgColor.Value.x, bgColor.Value.y, bgColor.Value.z, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);

		//Update dynamic object position
		quad->position.x += (-input_left + input_right)*300.0f*deltaTime;
		quad->position.y += (-input_down + input_up)*300.0f*deltaTime;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(quad->position.x, quad->position.y, 0.0f));
		//Drawing shapes
		glUseProgram(defaultShader->shaderProgram);
		defaultShader->SetMat4x4("uMvp", &(proj*view*model)[0][0]);

		defaultShader->SetUniform1f("uTime", uTime);
		defaultShader->SetVector2("uResolution", static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
		defaultShader->SetVector2("uMouse", (float)mousePosX, (float)(SCREEN_HEIGHT - mousePosY));

		tex.Bind(0);
		defaultShader->SetUniform1i("uTex0", 0);
		
		quad->Draw();

		
		glUseProgram(lightSurfaceShader->shaderProgram);
		lightSurfaceShader->SetVector2("uResolution", (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		lightSurfaceShader->SetVector2("uMouse", (float)mousePosX, (float)(SCREEN_HEIGHT - mousePosY));
		lightSurfaceShader->SetMat4x4("uMvp", &(proj * view)[0][0]);
		//lightSurface->Draw();
	
		//using gragrab pass texture; 
		glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		glUseProgram(grabPassShader->shaderProgram);
		grabPassShader->SetVector2("uResolution", (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		grabPassShader->SetVector2("uMouse", (float)mousePosX, (float)(SCREEN_HEIGHT - mousePosY));
		grabPassShader->SetUniform1f("uTime", (float)uTime);

		grabPassShader->SetMat4x4("uMvp", &(proj * view)[0][0]);
		grabTex.UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 0);
		grabTex.Bind(1);
		grabPassShader->SetUniform1i("uTex0", 1);
		grabPass->Draw();



		//Render Im::Gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Update screen
		glfwSwapBuffers(window);
		glfwPollEvents();
		//Calculate fps
		currentTime = glfwGetTime();
		deltaTime = currentTime- lastTime;
		lastTime = currentTime; //Well it's negligeable operation
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}