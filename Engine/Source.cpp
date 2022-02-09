/*


					 /$$   /$$ /$$      /$$                               /$$
					| $$$ | $$| $$  /$ | $$                              |__/
					| $$$$| $$| $$ /$$$| $$  /$$$$$$  /$$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$
					| $$ $$ $$| $$/$$ $$ $$ /$$__  $$| $$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
					| $$  $$$$| $$$$_  $$$$| $$$$$$$$| $$  \ $$| $$  \ $$| $$| $$  \ $$| $$$$$$$$
					| $$\  $$$| $$$/ \  $$$| $$_____/| $$  | $$| $$  | $$| $$| $$  | $$| $$_____/
					| $$ \  $$| $$/   \  $$|  $$$$$$$| $$  | $$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
					|__/  \__/|__/     \__/ \_______/|__/  |__/ \____  $$|__/|__/  |__/ \_______/
										    /$$  \ $$
										   |  $$$$$$/
										    \______/

																														*/

#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Interface.h"
#include "Globals.h"
#include "Primitives.h"
#include "Inputs.h"
#include "Maths.h"
#include "Game.h"
#include "Texture.h"
#include "Text.h"
#include "RessourcesLoader.h"
#include "Camera.h"
#include "Utilities.h"
#include "TileMap.h"
#include "Scene.h"
#include "Audio.h"
//Include game scripts
#include "Scripts/player.h"

//Variables
double fps = 60;
int frameCount = 0;
double currentTime;
double lastTime;
double deltaTimeSum = 0;

Camera camera = Camera(-(float)SCREEN_WIDTH / 2.0f, (float)SCREEN_WIDTH / 2.0f, -(float)SCREEN_HEIGHT / 2.0f, (float)SCREEN_HEIGHT / 2.0f);


float d1, d2 = 0.0f;

float pitch = 1.0f;


Collider groundCollider[20];
int main()
{
    //Init GLFW context 
    GLFWwindow* window = InitContext(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == nullptr) return -1;

	//init imgui
    InitInterface((int)window);
	//init OpenAL
	if (InitOpenAL() == 0) return -1;
	//const ALCchar* a = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	//std::cout << a << std::endl;

	ALuint snd = LoadSound("Ressources/Sounds/Mystery.wav");
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, snd);
	alSourcef(source, AL_GAIN, 0.0);
	alSourcei(source, AL_LOOPING, 1);
	alSourcePlay(source);

	//Load ressources
	LoadImages();
	LoadTextures();
	LoadShaders();
	LoadGameObjects();



	//Font font = Font("Ressources/fonts/rockstar.otf");





	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lastTime = glfwGetTime();
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //NANI!?
	TileMap tmt = TileMap("T1",Vector2<int>(32,32));
	tmt.tileObjects[0] = &objects["groundTile0"];
	tmt.tileObjects[1] = &objects["groundTile1"];
	tmt.tileObjects[2] = &objects["groundTile2"];
	tmt.tileObjects[3] = &objects["wallTile0"];

	for (int i = 0; i <15; i++)
	{
		groundCollider[i] = Collider(&(tmt.currentTileMap->tiles[i]) );
	}
	
	//DEBUG ZONE----------------------

	//END ZONE

	GameObjectClone postProcessing = GameObjectClone(&objects["postProcessing"]);


	Scene scene0 = Scene("scene0");


	scene0.LoadScene();


	Collider playerCol = Collider(&scene0.sceneObjs[7]);
	player pl = player(&scene0.sceneObjs[7]);
	

	
	double currentSprite = 0.0;
	bool isActive = true;
	const ImVec2 guiImageSize = ImVec2(50.0F, 50.0F);
	float yspd = 0;
	while (!glfwWindowShouldClose(window)){
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		UpdateInferface();
		//Debug--------------
		

		alSourcef(source, AL_PITCH, pitch);
	
		//------------------
		uTime += deltaTime;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		//BindTextures
		textures["tex"].Bind(0);
		textures["grabTex"].Bind(1);
		//TODO::Shader managment when it comes to draw shapes
		// 
		//Update dynamic object position
		//yspd -=  deltaTime;

		camera.Update();
		tmt.Update();
		pl.Update();

		currentSprite += deltaTime *5.0;
		//Drawing shapes


	/*	for (auto& it : drawOrder) {
			(it.second.first)->Draw(it.second.second);
		}*/

		scene0.Draw();
		tmt.Draw();

		//ground->Draw(0);

		if (pl.isRunning == 0) {
			textures["warriorTex"].UpdateTexture(textures["warriorTex"].size.x, textures["warriorTex"].size.y, IMAGES_WARRIOR_IDLE_ARRAY[((int)currentSprite) % 6]->tex, 0, 1);
		}
		else {
			textures["warriorTex"].UpdateTexture(textures["warriorTex"].size.x, textures["warriorTex"].size.y, IMAGES_WARRIOR_RUN_ARRAY[((int)currentSprite) % 8]->tex, 0, 1);
		}

		//scene0.sceneObjs[7].Draw(0);
		//objects["warrior"].Draw(0);

		d1 = playerCol.GetPosition().y;
		//s64.position = playerCol.GetPosition();
		//s64.Draw();
		//for (int i = 0; i < 15; i++) {
		//	s32.position = groundCollider[i].GetPosition();
		//	s32 .Draw();
		//}
	

		glReadPixels(-camera.position.x, -camera.position.y, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		textures["grabTex"].UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		objects["grabPass"].Draw(1);
		postProcessing.position = camera.position;
		glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		textures["grabTex"].UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		glClearColor(0.0, 0.0, 0.01, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);

		float offsetX = (WINDOW_WIDTH - RENDERING_WIDTH)*0.5f;
		float offsetY = (WINDOW_HEIGHT - RENDERING_HEIGHT)*0.5f;
		glViewport(offsetX,offsetY,RENDERING_WIDTH, RENDERING_HEIGHT);

		//objects["lightSurface"].Draw(1);
		//NO NEED to scale if you change viewport
	/*	postProcessing.scale.x = (float)(RENDERING_WIDTH) / (float)SCREEN_WIDTH;
		postProcessing.scale.y = (float)(RENDERING_HEIGHT) / (float)SCREEN_HEIGHT;*/
		postProcessing.Draw(1);

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


		//Drawing debug things and tilemap grid

		//Render Im::Gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Update screen
		glfwSwapBuffers(window);
		glfwPollEvents();
		//Calculate fps
		if (DEBUG_MODE) {
			frameCount += 1;
			deltaTimeSum += deltaTime;
			if (frameCount == 60) {
				fps = 60.0 / deltaTimeSum;
				deltaTimeSum = 0;
				frameCount = 0;
			}
		}
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime; //Well it's negligeable operation
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
};
