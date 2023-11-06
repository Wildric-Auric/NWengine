#include "Game.h"
#include "GL/glew.h"
#include "glfw3.h"

int8 Game::Run() {

	GLFWwindow* window = (GLFWwindow*)Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
	if (window == nullptr) return -1;
	if (!SoundSystem::InitOpenAL()) return -1;
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
		NWTime::Update();
	}
}


void Game::Shutdown() {
	delete Scene::currentScene;
	delete Renderer::defaultRenderer;

	ScriptManager::SaveScriptList();
	SoundSystem::DestroyOpenAL();
	TextSystem::Destroy();
	Primitives::Destroy();
	Context::Destroy();
}

