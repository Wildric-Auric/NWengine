#include "NWengine.h"
#include "Game.h"
#include "GL/glew.h"
#include "glfw3.h"

int8 Game::Run() {

	GLFWwindow* window = (GLFWwindow*)Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
	if (window == nullptr) return -1;

	//init imgui
	Gui::Init((void*)window);
	//init OpenAL
	if (!InitOpenAL()) return -1;

	//Load ressources
	RessourcesLoader::LoadDefaultRessources();

	Context::EnableBlend();

	////Initialization finished
	//TODO::UI for scene load ans serialization
	(Scene::currentScene = new Scene("scene0"))->LoadScene();

	//Initializing Game class

	GameObject renderObj	  = GameObject();
	Renderer::defaultRenderer = new Renderer(&renderObj); //Unchanged by the user
	Renderer::currentRenderer = Renderer::defaultRenderer;

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

	static GameObject go = GameObject();
	static Camera* cam    = go.AddComponent<Camera>();

	while (!glfwWindowShouldClose((GLFWwindow*)Context::window)) {
		Context::Clear();

		Inputs::Process(Context::window);

		Globals::uTime += Globals::deltaTime;

		//Drawing shapes
		Scene::currentScene->Update();

		if (Camera::ActiveCamera != nullptr) {
			Camera::ActiveCamera->Update();
			Camera::ActiveCamera->Capture();
		}
		//Final frame
		Renderer::currentRenderer->DrawOnDefaultFrame();
		//Update screen
		Context::Update();
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
	delete Scene::currentScene;
	delete Renderer::defaultRenderer;

	DestroyOpenAL();
	ImGui_ImplOpenGL3_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
	glfwTerminate();
}

