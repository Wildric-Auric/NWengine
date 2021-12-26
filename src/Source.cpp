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
double fps = 60;
int frameCount = 0;
double currentTime;
double lastTime;
double deltaTimeSum = 0;


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
	Texture warriorTex = Texture(IMAGE_WARRIOR_IDLE_1.width, IMAGE_WARRIOR_IDLE_1.height, IMAGE_WARRIOR_IDLE_1.tex, 1, 0);
	Texture backgroundTex = Texture(IMAGE_BACKGROUND.width, IMAGE_BACKGROUND.height, IMAGE_BACKGROUND.tex, 1, 1);
	Texture bush1Tex = Texture(IMAGE_BUSH1.width, IMAGE_BUSH1.height, IMAGE_BUSH1.tex, 1, 0);
	Texture bush2Tex = Texture(IMAGE_BUSH2.width, IMAGE_BUSH2.height, IMAGE_BUSH2.tex, 1, 0);
	Texture tree1Tex = Texture(IMAGE_TREE1.width, IMAGE_TREE1.height, IMAGE_TREE1.tex, 1, 0);
	Texture tree2Tex = Texture(IMAGE_TREE2.width, IMAGE_TREE2.height, IMAGE_TREE2.tex, 1, 0);
	Texture groundTex = Texture(IMAGE_GROUND.width, IMAGE_GROUND.height, IMAGE_GROUND.tex, 1, 0);


	LoadShaders();

	



	//Shader* textShader			= new Shader();

	GameObject* lesbeanApple = (GameObject*)malloc(sizeof(GameObject)); //Need default constructor to use ew; I'm lazy, I can't do it now
	GameObject* lesbeanApple2 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* grabPass = (GameObject*)malloc(sizeof(GameObject));
	GameObject* lightSurface	= (GameObject*)malloc(sizeof(GameObject));
	GameObject* postProcessing = (GameObject*)malloc(sizeof(GameObject));
	GameObject* warrior = (GameObject*)malloc(sizeof(GameObject));
	GameObject* background = (GameObject*)malloc(sizeof(GameObject));
	GameObject* background1 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* background2 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* background3 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* background4 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* tree1 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* tree2 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* bush1 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* bush2 = (GameObject*)malloc(sizeof(GameObject));
	GameObject* ground = (GameObject*)malloc(sizeof(GameObject));




	//Text* text	= new Text();
	//int	init	= text->initfreetype("fonts/rockstar.otf");


	*lesbeanApple = GameObject(&tex, Vector2<int>(0,0), Vector2<float>(0.5f,0.5f), shader_default);					//Quad(Vector2<int>(0 ,0 ), 300.0F, 300.0F);
	*lesbeanApple2 = GameObject(&tex, Vector2<int>(200, 100), Vector2<float>(-.5f, .5f), shader_default);
	*grabPass	= GameObject(&grabTex, Vector2<int>(0 , 0 ), Vector2<float>(1.0f,1.0f),
						shader_grabPass);
	*lightSurface   = GameObject(&grabTex, Vector2<int>(0 , 0 ), Vector2<float>(1.0f, 1.0f),
						shader_lightSurface);
	*postProcessing = GameObject(&grabTex, Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f),
						shader_postProcessing);
	*warrior = GameObject(&warriorTex, Vector2<int>(0, -20), Vector2<float>(1.0f, 1.0f));
	float s = 2.2;
	*background = GameObject(&backgroundTex, Vector2<int>(-337, 130), Vector2<float>(s, s));
	*background1 = GameObject(&backgroundTex, Vector2<int>(-337 + background->size.x, 130), Vector2<float>(s, s));
	*background2 = GameObject(&backgroundTex, Vector2<int>(-337 + 2 *background->size.x, 130), Vector2<float>(s, s));
	*background3 = GameObject(&backgroundTex, Vector2<int>(-337 + 3 * background->size.x, 130), Vector2<float>(s, s));
	*background4 = GameObject(&backgroundTex, Vector2<int>(-337 + 4 * background->size.x, 130), Vector2<float>(s, s));
	s = 2.0f;
	*bush1 = GameObject(&bush1Tex, Vector2<int>(113, -28), Vector2<float>(s, s));
	*bush2 = GameObject(&bush2Tex, Vector2<int>(-119, -28), Vector2<float>(s, s));
	*tree1 = GameObject(&tree1Tex, Vector2<int>(156, 12), Vector2<float>(s, s));
	*tree2 = GameObject(&tree2Tex, Vector2<int>(-212, 19), Vector2<float>(s, s));
	s = 1.5f;
	*ground = GameObject(&groundTex, Vector2<int>(-100, -40), Vector2<float>(s, s));

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

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //NANI!?

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
		ImGui::SliderInt2("bg", &background->position.x, -400, 400);
		ImGui::SliderInt2("tree1", &tree1->position.x, -400, 400);
		ImGui::SliderInt2("tree2", &tree2->position.x, -400, 400);
		ImGui::SliderInt2("bush1", &bush1->position.x, -400, 400);
		ImGui::SliderInt2("bush2", &bush2->position.x, -400, 400);
		ImGui::SliderInt2("ground", &ground->position.x, -400, 400);


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

		camera.Update();
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
		ground->Draw(0);
		warriorTex.UpdateTexture(warriorTex.size.x, warriorTex.size.y, IMAGES_WARRIOR_IDLE_ARRAY[((int)currentSprite) % 6]->tex, 0, 1);
		warrior->Draw(0);
		lesbeanApple->Draw(0);
		lesbeanApple2->Draw(0);
		glReadPixels(-camera.position.x, -camera.position.y, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);

		grabTex.UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		grabPass->Draw(1);
		glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels); 
		grabTex.UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		postProcessing->Draw(1);
		

		//lightSurface->Draw(1);




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
	free(lightSurface);
	free(lesbeanApple);
	free(grabPass);
}