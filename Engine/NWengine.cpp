#include "NWengine.h"
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

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			Context::Clear();
			Inputs::Process((GLFWwindow*)Context::window);
			Gui::Update();
			Globals::uTime += Globals::deltaTime;
			if (Camera::ActiveCamera != nullptr)
				Camera::ActiveCamera->Capture();

			if (Scene::currentScene != nullptr)
				Scene::currentScene->Update();

			Renderer::currentRenderer->CaptureOnCamFrame();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapInterval(1);
			glfwSwapBuffers((GLFWwindow*)Context::window);
			glfwPollEvents();

			//Updating dll flag
			if (glfwWindowShouldClose((GLFWwindow*)Context::window)) Context::dllFlag = NW_SHUTDOWN_DLL;

			frameCount += 1;
			deltaTimeSum += Globals::deltaTime;
			if (frameCount == 60) {
				Globals::fps = 60.0 / deltaTimeSum;
				deltaTimeSum = 0;
				frameCount = 0;
			}
			currentTime = glfwGetTime();
			Globals::deltaTime = currentTime - lastTime;
			lastTime = currentTime;
		}
		return Context::dllFlag;
	}

	__declspec(dllexport)  int DllRun() {
		GLFWwindow* window = (GLFWwindow*)Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
		Gui::Init((void*)window);
		if (!InitOpenAL()) return -1;
		RessourcesLoader::LoadDefaultRessources();
		ScriptManager::LoadScriptList();
		Context::EnableBlend();
		SceneEditor::Init();
		(Scene::currentScene = new Scene("scene0"))->LoadScene();
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

		//init imgui
		Gui::Init((void*)window);
		//init OpenAL
		if (!InitOpenAL()) return -1;
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
		
		Batch::maxBatchTextures = 32; //TODO::Make a function that uses glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS ...
	
		////Initialization finished

		(Scene::currentScene = new Scene("scene0"))->LoadScene();
		
		
		
		NWengine::MainLoop();

		NWengine::Shutdown();
		return 0;
}


//------------------
void NWengine::MainLoop() {

	int frameCount = 0;
	double currentTime;
	double lastTime;
	double deltaTimeSum = 0;
	lastTime = glfwGetTime();

	GameObject renderObj = GameObject();
	Renderer::defaultRenderer = new Renderer(&renderObj); //Unchanged by the user
	Renderer::currentRenderer = Renderer::defaultRenderer;

	while (!glfwWindowShouldClose((GLFWwindow*)Context::window)) {
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Clearing for the UI
		Context::Clear();	
		Inputs::Process((GLFWwindow*)Context::window);
		Gui::Update();

		//Updating camera framebuffer
		if (Camera::ActiveCamera != nullptr) {
			Camera::ActiveCamera->Capture();
		}

		//Updating Renderer camera framebuffer
		Renderer::currentRenderer->CaptureOnCamFrame();

		//Updating scene objects
		if (Scene::currentScene != nullptr)
			Scene::currentScene->Update();

		//Render Im::Gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Update window and capturing inputs
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


void NWengine::Shutdown() {
		delete Scene::currentScene;
		delete Renderer::defaultRenderer;
		delete SceneEditor::cam;

		ScriptManager::SaveScriptList();
		DestroyOpenAL();

		TextSystem::Destroy();
		Primitives::Destroy();

		ImGui_ImplOpenGL3_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		glfwTerminate();
}

#endif