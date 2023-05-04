#include "Game.h"
#include "GL/glew.h"
#include "glfw3.h"

int8 Game::Run() {

	GLFWwindow* window = (GLFWwindow*)Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
	if (window == nullptr) return -1;
	if (!InitOpenAL()) return -1;
	if (!TextSystem::Init())
		return -1;
	Primitives::Init();
	RessourcesLoader::LoadDefaultRessources();
	ScriptManager::LoadScriptList();
	Context::EnableBlend();
	Batch::ComputeIndices();
	Batch::maxBatchTextures = 32;
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

	if (Scene::currentScene != nullptr)
		Scene::currentScene->Start();

	GameObject renderObj = GameObject();
	Renderer::defaultRenderer = new Renderer(&renderObj); //Unchanged by the user
	Renderer::currentRenderer = Renderer::defaultRenderer;

	(Scene::currentScene = new Scene(NW_DEFAULT_SCENE))->LoadScene();
	Scene::currentScene->Start();
	while (!glfwWindowShouldClose((GLFWwindow*)Context::window)) {
		Context::Clear();

		Inputs::Process(Context::window);

		Globals::uTime += Globals::deltaTime;

		if (Camera::ActiveCamera != nullptr) {
			Camera::ActiveCamera->Capture();
		}

		if (Scene::currentScene != nullptr)
			Scene::currentScene->Update();
		//Final frame
		Renderer::currentRenderer->DrawOnDefaultFrame();
		//Update screen
		Context::Update();
		//Calculate fps
		frameCount += 1;
		deltaTimeSum += Globals::deltaTime;
		if (frameCount == 60) {
			Globals::fps = 60.0 / deltaTimeSum;
			deltaTimeSum = 0;
			frameCount = 0;
		}

		currentTime = glfwGetTime();
		Globals::deltaTime = currentTime - lastTime;
		lastTime = currentTime; //Well it's negligeable operation
	}
}


void Game::Shutdown() {
	delete Scene::currentScene;
	delete Renderer::defaultRenderer;

	ScriptManager::SaveScriptList();
	DestroyOpenAL();
	TextSystem::Destroy();
	Primitives::Destroy();
	Context::Destroy();
}

