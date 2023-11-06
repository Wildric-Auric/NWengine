#include "NWengine.h"
#include "NWGui.h"
#include "Gui.h"
#include "SceneEditor.h"
#include "GL/glew.h"
#include "glfw3.h"

#ifdef _WINDLL
extern "C"
{
	int DllLoop() {
		int frameCount = 0;
		double currentTime;
		double lastTime;
		double deltaTimeSum = 0;
		lastTime = glfwGetTime();

		GameObject renderObj = GameObject();
		Renderer::defaultRenderer = new Renderer(&renderObj); //Unchanged by the user
		Renderer::currentRenderer = Renderer::defaultRenderer;

		Context::dllFlag = NW_KEEP_DLL_RUNNING;
		while (Context::dllFlag == NW_KEEP_DLL_RUNNING) {
			Gui::Begin();
			Context::Clear();
			Inputs::Process((GLFWwindow*)Context::window);
			Gui::Update();
			Camera::Update();
			Scene::Update();
			Renderer::currentRenderer->CaptureOnCamFrame();

			Gui::Render();

			Context::Update();
			//Updating dll flag
			if (glfwWindowShouldClose((GLFWwindow*)Context::window)) Context::dllFlag = NW_SHUTDOWN_DLL;
			NWTime::Update();
		}
		return Context::dllFlag;
	}

	__declspec(dllexport)  int DllRun() {
		GLFWwindow* window = (GLFWwindow*)Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);

		Globals::SetInstallationDir(GetCurrentDir());
		(NWproj::currentProj = new NWproj(NWproj::GetCurrentProjFromInstallationDir()))->Load();
		Globals::SetProjDir(NWproj::currentProj->dir);

		Gui::Init((void*)window);
		if (!SoundSystem::InitOpenAL()) return -1;
		if (!TextSystem::Init())
			return -1;
		Primitives::Init();
		RessourcesLoader::LoadDefaultRessources();
		ScriptManager::LoadScriptList();
		Context::EnableBlend();
		SceneEditor::Init();
		Batch::ComputeIndices();
		Batch::maxBatchTextures = 32; 

		////Initialization finished
		if (NWproj::currentProj != nullptr && NWproj::currentProj->defaultScenePath != "")
			(Scene::currentScene = new Scene(NWproj::currentProj->defaultScenePath))->LoadScene();
	    DllLoop();
		NWengine::Shutdown();
		return Context::dllFlag;
	}
}

#endif

#ifndef NW_DLL_ENGINE
int NWengine::Run() {
		GLFWwindow* window = (GLFWwindow*)Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
		if (window == nullptr) return -1;
		//Init project

		Globals::SetInstallationDir(GetCurrentDir());
		(NWproj::currentProj = new NWproj(NWproj::GetCurrentProjFromInstallationDir()))->Load();
		Globals::SetProjDir(NWproj::currentProj->dir);

		//init imgui
		Gui::Init((void*)window);
		//init OpenAL
		if (!SoundSystem::InitOpenAL()) return -1;
		//init freetype
		if (!TextSystem::Init())
			return -1;
		

		Primitives::Init();
		//Load ressources
		RessourcesLoader::LoadDefaultRessources();
		//Loading script list
		ScriptManager::LoadScriptList();
		//Context settings

		Context::EnableBlend();

		SceneEditor::Init();
		
		Batch::ComputeIndices();
		
		Batch::maxBatchTextures = 32; //TODO::Make a function that uses glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS...

		////Initialization finished
		if (NWproj::currentProj != nullptr && NWproj::currentProj->defaultScenePath != "")
			(Scene::currentScene = new Scene(NWproj::currentProj->defaultScenePath))->LoadScene();
		
		NWengine::MainLoop();

		NWengine::Shutdown();
		return 0;
}


//------------------
void NWengine::MainLoop() {


	GameObject renderObj = GameObject();
	Renderer::defaultRenderer = new Renderer(&renderObj); //Unchanged by the user
	Renderer::currentRenderer = Renderer::defaultRenderer;

	while (!glfwWindowShouldClose((GLFWwindow*)Context::window)) {
		Gui::Begin();
		//Clearing for the UI
		Context::Clear();	
		Inputs::Process((GLFWwindow*)Context::window);
		Gui::Update();
		Camera::UpdateActiveCamera();
		Scene::UpdateActiveScene();
		//Updating Renderer camera framebuffer
		Renderer::currentRenderer->CaptureOnCamFrame();
		//Render Im::Gui
		Gui::Render();
		//Update window and capturing inputs
		Context::Update();
		//Calculate fps
		NWTime::Update();
	}
}


void NWengine::Shutdown() {
		delete Scene::currentScene;
		delete Renderer::defaultRenderer;
		delete SceneEditor::cam;
		delete NWproj::currentProj;

		ScriptManager::SaveScriptList();
		SoundSystem::DestroyOpenAL(); 

		TextSystem::Destroy();
		Primitives::Destroy();

		Gui::Destroy();
		Context::Destroy();
}

#endif