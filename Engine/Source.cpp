#include "NWengine.h"
#include "FrameBuffer.h"
//Include game scripts
//#include "Scripts/player.h"

//Variables
double fps = 60;
int frameCount = 0;
double currentTime;
double lastTime;
double deltaTimeSum = 0;

Camera camera = Camera(-(float)ORIGINAL_WIDTH / 2.0f, (float)ORIGINAL_WIDTH / 2.0f, -(float)ORIGINAL_HEIGHT / 2.0f, (float)ORIGINAL_HEIGHT / 2.0f);
float pitch = 1.0f;
float uniformTest = 1;
Collider groundCollider[20];
void EnableWireframe(bool status) {
	if (status){
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		
	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

int main()
{
    //Init GLFW context 
    GLFWwindow* window = InitContext(ORIGINAL_WIDTH, ORIGINAL_HEIGHT);
	if (window == nullptr) return -1;

	//init imgui
    InitInterface((int)window);
	//init OpenAL
	if (InitOpenAL() == 0) return -1;

	ALuint snd = LoadSound("Ressources/Sounds/Mystery.wav");
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, snd);
	alSourcef(source, AL_GAIN, 0.0);
	alSourcei(source, AL_LOOPING, 1);
	alSourcePlay(source);


	//init frame buffer
	FrameBuffer fbo = FrameBuffer();
	//Load ressources
	LoadRessources();



	//Font font = Font("Ressources/fonts/rockstar.otf");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lastTime = glfwGetTime();
	
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
		

	
		//------------------
		uTime += deltaTime;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		//BindTextures
		textures["tex"].Bind(0);
		textures["grabTex"].Bind(1);

		camera.Update();
		tmt.Update();

		currentSprite += deltaTime *5.0;
		//Drawing shapes
		scene0.Draw();

		tmt.Draw();

		//Update Scripts

		for (auto it = Script::componentList.begin(); it != Script::componentList.end(); it++) {
			it->second.script->Update();
		}


		glReadPixels(-camera.position.x, -camera.position.y, ORIGINAL_WIDTH, ORIGINAL_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		textures["grabTex"].UpdateTexture(ORIGINAL_WIDTH, ORIGINAL_HEIGHT, behindPixels, 1);
		objects["grabPass"].Draw(1);
		postProcessing.position = camera.position;

		glReadPixels(0, 0, ORIGINAL_WIDTH, ORIGINAL_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		textures["grabTex"].UpdateTexture(ORIGINAL_WIDTH, ORIGINAL_HEIGHT, behindPixels, 1);

		glClearColor(0.0, 0.0, 0.01, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);

		fbo.Bind();
		glUseProgram(postProcessing.originalGameObject->shader->shaderProgram);
		postProcessing.originalGameObject->texture->Bind(1);
		postProcessing.originalGameObject->shader->SetUniform1i("uTex0", 1);
		postProcessing.originalGameObject->shader->SetUniform1f("uCells", uniformTest);
		postProcessing.BasicDraw(1);
		fbo.Unbind();
	
	

		



		glClear(GL_COLOR_BUFFER_BIT);
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
