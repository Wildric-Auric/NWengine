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
				Camera::ActiveCamera->Capture(0.0, 0.0, 0.1);
			
			Scene::currentScene->Update();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapInterval(1);
			glfwSwapBuffers((GLFWwindow*)Context::window);
			glfwPollEvents();

			//Updating dll flag
			if (glfwWindowShouldClose((GLFWwindow*)Context::window)) Context::dllFlag = NW_SHUTDOWN_DLL;

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
		Context::EnableDepthTest();
		Scene* scene0 = new Scene("scene0");
		scene0->LoadScene();
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

		//Load ressources
		RessourcesLoader::LoadDefaultRessources();
		//Loading script list
		ScriptManager::LoadScriptList();
		//Context settings

		Context::EnableBlend();

		Context::EnableDepthTest();
		
		////Initialization finished
		//TODO::UI for scene load ans serialization
		Scene* scene0 = new Scene("scene0");
		scene0->LoadScene();
		
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

	while (!glfwWindowShouldClose((GLFWwindow*)Context::window)) {
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Clearing for the UI
		Context::Clear();	

		Inputs::Process((GLFWwindow*)Context::window);
		Gui::Update();

		Globals::uTime += Globals::deltaTime;

		//Drawing shapes
		if (Camera::ActiveCamera != nullptr) {
			Camera::ActiveCamera->Capture(0.0, 0.0, 0.1);
		}

		Scene::currentScene->Update(); //Leak

		//Render Im::Gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Update screen
		glfwSwapInterval(1);
		glfwSwapBuffers((GLFWwindow*)Context::window);
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


void NWengine::Shutdown() {
		delete Scene::currentScene;
		ScriptManager::SaveScriptList();
		DestroyOpenAL();
		ImGui_ImplOpenGL3_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		glfwTerminate();
}

#endif