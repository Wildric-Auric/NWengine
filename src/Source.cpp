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
//#include "ShaderManager.h"
#include "Inputs.h"
#include "Maths.h"
#include "Game.h"
#include "Texture.h"
//#include "Text.h"
#include "RessourcesLoader.h"
//#include  "GameObject.h"
#include "Globals.h"
#include "Camera.h"
#include "Utilities.h"
#include "TileMap.h"
//Variable
double fps = 60;
int frameCount = 0;
double currentTime;
double lastTime;
double deltaTimeSum = 0;

Camera camera = Camera(-(float)SCREEN_WIDTH / 2.0f, (float)SCREEN_WIDTH / 2.0f, -(float)SCREEN_HEIGHT / 2.0f, (float)SCREEN_HEIGHT / 2.0f);

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
	LoadImages();
	LoadTextures();
	LoadShaders();
	LoadGameObjects();





	//Shader* textShader			= new Shader();
	//Text* text	= new Text();
	//int	init	= text->initfreetype("fonts/rockstar.otf");


	/*Collider collider_apple = Collider(lesbeanApple);
	Collider collider_apple2 = Collider(lesbeanApple2);*/
	Collider collider_water = Collider(grabPass);
	


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lastTime = glfwGetTime();
	ImColor bgColor = ImColor(82, 75, 108);

	irrklang::ISound* sd = SoundEngine->play2D("Ressources/Sounds/Mystery.mp3", true, false, true);
	if (sd) sd->setVolume(0.3);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //NANI!?
	TileMap tmt = TileMap("T1",Vector2<int>(32,32));
	std::cout << allObjects[16]->position.x << std::endl;



	//GameObjectClone instancingApple = GameObjectClone(lesbeanApple);

	double currentSprite = 0.0;
	while (!glfwWindowShouldClose(window)){
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::Text("fps = %f", fps);
		ImGui::ColorEdit3("Background Color", (float*)&bgColor);
		ImGui::SliderInt2("WarriorPos", &warrior->position.x, -400, 400);
		//ImGui::SliderInt2("bg", &background->position.x, -400, 400);
		//ImGui::SliderInt2("tree1", &tree1->position.x, -400, 400);
		//ImGui::SliderInt2("tree2", &tree2->position.x, -400, 400);
		//ImGui::SliderInt2("bush1", &bush1->position.x, -400, 400);
		//ImGui::SliderInt2("bush2", &bush2->position.x, -400, 400);
		//ImGui::SliderInt2("ground", &ground->position.x, -400, 400);
		//ImGui::SliderFloat2("appleSize", &(instancingApple.scale.x), -3.0f, 3.0f);


		ImGui::End();
		//Debug--------------
		//std::cout << lesbeanApple->size.x << "  " << collider_apple2.size->x << std::endl;
		//if (isColliding(&collider_apple, &collider_apple2)) std::cout << "Hello Collision" << std::endl;
		
		//------------------
		uTime += deltaTime;
		glClearColor(bgColor.Value.x, bgColor.Value.y, bgColor.Value.z, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		//BindTextures
		tex->Bind(0);
		grabTex->Bind(1);

		//TODO::Shader managment when it comes to draw shapes
		// 
		//Update dynamic object position
		lesbeanApple->position.x += (-input_left + input_right)*300.0f*deltaTime;
		lesbeanApple->position.y += (-input_down + input_up)*300.0f*deltaTime;
		//while (IsColliding(&collider_apple, &collider_apple2)) {
		//	lesbeanApple->position.x -= (-input_left + input_right) * 300.0f * deltaTime;
		//	lesbeanApple->position.y -= (-input_down + input_up) * 300.0f * deltaTime;
		//}
		camera.position = lesbeanApple->position;

		camera.Update();
		tmt.Update();
		//Drawing shapes
		currentSprite += deltaTime *5.0;
		//calling this every frame is not optimal
		background->Draw(0);
		background1->Draw(0);
		background2->Draw(0);
		background3->Draw(0);
		background4->Draw(0);
		bush1->Draw(0);
		bush2->Draw(0);
		tree1->Draw(0);
		tree2->Draw(0);

		for (auto it = tmt.tiles.begin(); it != tmt.tiles.end(); ++it) {
			//std::cout << tmt.tiles[0].originalGameObject->position.x<< std::endl;
			//std::cout << tmt.tiles[1].position.x << std::endl;
			(*it).Draw(0);
		}

		//ground->Draw(0);
		warriorTex->UpdateTexture(warriorTex->size.x, warriorTex->size.y, IMAGES_WARRIOR_IDLE_ARRAY[((int)currentSprite) % 6]->tex, 0, 1);
		warrior->Draw(0);
		//lesbeanApple->Draw(0);
		//instancingApple.Draw(0);
		//lesbeanApple2->Draw(0);
		glReadPixels(-camera.position.x, -camera.position.y, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		grabTex->UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		grabPass->Draw(1);
		postProcessing->position = camera.position;
		glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		grabTex->UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		postProcessing->Draw(1);
		//lightSurface->Draw(1);


		//Drawing debug things and tilemap grid
		//tmt.RenderGrid();


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