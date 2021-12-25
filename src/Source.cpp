#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <irrKlang.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Primitives.h"
#include "ShaderManager.h"
#include "Inputs.h"
#include "Maths.h"
#include "Game.h"
#include "Texture.h"
//#include "Text.h"
#include "RessourcesLoader.h"
#include  "GameObject.h"
#include "Globals.h"
#include "Camera.h"
#include "Utilities.h"
//Variable
float fps = 60;
int frameCount = 0;

double currentTime;
double lastTime;


using namespace irrklang;

int main()
{ 

    //Init GLFW context 
	GLFWwindow* window = InitContext(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == nullptr) return -1;

	//Create ImGui context
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//init irrKlang
	ISoundEngine* SoundEngine = createIrrKlangDevice();


	//Load ressources
	GLubyte* behindPixels = new GLubyte[4 * SCREEN_WIDTH * SCREEN_HEIGHT];
	LoadImages();
	Texture tex = Texture(IMAGE_APPLE.width, IMAGE_APPLE.height, IMAGE_APPLE.tex, 1, 0);
	Texture grabTex = Texture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels);
	LoadShaders();

	



	//Shader* textShader			= new Shader();

	GameObject* lesbeanApple = (GameObject*)malloc(sizeof(GameObject)); //Need default constructor to use new; I'm lazy, I can't do it now
	GameObject* lesbeanApple2 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* grabPass = (GameObject*)malloc(sizeof(GameObject));
	GameObject* lightSurface	= (GameObject*)malloc(sizeof(GameObject));

	//Text* text	= new Text();
	//int	init	= text->initfreetype("fonts/rockstar.otf");


	*lesbeanApple = GameObject(&tex, Vector2<int>(0,0), Vector2<float>(1.0f,1.0f), shader_default);					//Quad(Vector2<int>(0 ,0 ), 300.0F, 300.0F);
	*lesbeanApple2 = GameObject(&tex, Vector2<int>(500, 0), Vector2<float>(-1.0f, 1.0f), shader_default);
	*grabPass	= GameObject(&grabTex, Vector2<int>(0 , 0 ), Vector2<float>(1.0f,1.0f),
						shader_grabPass);

	*lightSurface   = GameObject(&grabTex, Vector2<int>(0 , 0 ), Vector2<float>(1.0f, 1.0f),
						shader_lightSurface);

	Collider collider_apple = Collider(lesbeanApple);
	Collider collider_apple2 = Collider(lesbeanApple2);
	Collider collider_water = Collider(grabPass);


	Camera camera = Camera(-(float)SCREEN_WIDTH/2.0f,(float) SCREEN_WIDTH/2.0f, -(float) SCREEN_HEIGHT/2.0f, (float) SCREEN_HEIGHT/2.0f);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lastTime = glfwGetTime();
	ImColor bgColor = ImColor(82, 75, 108);

	irrklang::ISound* sd = SoundEngine->play2D("Ressources/Sounds/Mystery.mp3", true, false, true);
	if (sd) sd->setVolume(0.3);

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); NANI!?

	while (!glfwWindowShouldClose(window)){
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::ColorEdit3("Background Color", (float*)&bgColor);
		ImGui::End();
		//Debug--------------

		//if (isColliding(&collider_apple, &collider_apple2)) std::cout << "Hello Collision" << std::endl;

		//------------------
		uTime += deltaTime;
		glClearColor(bgColor.Value.x, bgColor.Value.y, bgColor.Value.z, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		//BindTextures
		tex.Bind(0);
		grabTex.Bind(1);
		//TODO::Shader managment when it comes to draw shapes

		//Update dynamic object position
		lesbeanApple->position.x += (-input_left + input_right)*300.0f*deltaTime;
		lesbeanApple->position.y += (-input_down + input_up)*300.0f*deltaTime;
		while (isColliding(&collider_apple, &collider_apple2)) {
			lesbeanApple->position.x -= (-input_left + input_right) * 300.0f * deltaTime;
			lesbeanApple->position.y -= (-input_down + input_up) * 300.0f * deltaTime;
		}

		camera.position = lesbeanApple->position;
		camera.Update();
		//Drawing shapes
		lesbeanApple->Draw(0);
		lesbeanApple2->Draw(0);
		glReadPixels(-camera.position.x, -camera.position.y, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		grabTex.UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		grabPass->Draw(1);
		//lightSurface->Draw(1);




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
	free(lightSurface);
	free(lesbeanApple);
	free(grabPass);
}