#include "NWengine.h"
#include "Game.h"

GameObject Game::FrameObject;
Sprite* Game::RenderedTexture;

int8 Game::Run() {

	GLFWwindow* window = Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
	if (window == nullptr) return -1;

	//init imgui
	Gui::Init((int)window);
	//init OpenAL
	if (!InitOpenAL()) return -1;

	//Load ressources
	RessourcesLoader::LoadDefaultRessources();

	Context::EnableBlend();

	////Initialization finished
	//TODO::UI for scene load ans serialization
	Scene scene0 = Scene("scene0");
	scene0.LoadScene();

	//Initializing Game class
	Game::FrameObject.AddComponent<Transform>();
	RenderedTexture = Game::FrameObject.AddComponent<Sprite>();
	MainLoop();

	Shutdown();
}

//------------------
void Game::MainLoop() {

	int frameCount = 0;
	double currentTime;
	double lastTime;
	double deltaTimeSum = 0;
	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(Context::window)) {
		Context::Clear();

		Inputs::Process(Context::window);

		Globals::uTime += Globals::deltaTime;

		//Drawing shapes
		Camera::ActiveCamera->Capture(0.2, 0.2, 0.2);

		Camera::ActiveCamera->Update();
		Scene::currentScene->Update();
		//Final frame
		RenderedTexture->texture = &Camera::ActiveCamera->fbo.RenderedImage;
		RenderedTexture->container = Quad(iVec2(0,0), RenderedTexture->texture->size.x, RenderedTexture->texture->size.y);
		Context::SetViewPort(0, 0, Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT);
		FrameObject.GetComponent<Transform>()->position = iVec2(Camera::ActiveCamera->position.x, Camera::ActiveCamera->position.y);
		Context::SetViewPort((Context::WINDOW_WIDTH - RenderedTexture->texture->size.x) * 0.5,
							Context::WINDOW_HEIGHT * 0.5 - RenderedTexture->texture->size.y * 0.5,
							RenderedTexture->texture->size.x, RenderedTexture->texture->size.y); //TEMP

		FrameObject.Draw();

		//Update screenSetTexture
		glfwSwapBuffers(Context::window);
		glfwPollEvents();
		//Calculate fps
		if (Globals::DEBUG_MODE) {
			frameCount += 1;
			deltaTimeSum += Globals::deltaTime;
			if (frameCount == 60) {
				Globals::fps = 60.0 / deltaTimeSum;
				deltaTimeSum = 0;
				frameCount = 0;
			}
		}
		currentTime = glfwGetTime();
		Globals::deltaTime = currentTime - lastTime;
		lastTime = currentTime; //Well it's negligeable operation
	}
}


void Game::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

