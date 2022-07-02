#include "NWengine.h"


int8 NWengine::Run() {

		int frameCount = 0;
		double currentTime;
		double lastTime;
		double deltaTimeSum = 0;


		GLFWwindow* window = InitContext(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT);
		if (window == nullptr) return -1;

		//init imgui
		Gui::Init((int)window);
		//init OpenAL
		if (!InitOpenAL()) return -1;


		//Load ressources
		RessourcesLoader::LoadDefaultRessources();

		EnableBlend(1);


		lastTime = glfwGetTime();
		////Initialization finished

		Scene scene0 = Scene("scene0");
		scene0.LoadScene();

		glClearColor(0.0f, 0.0f, 0.01f, 1.0);
		while (!glfwWindowShouldClose(window)) {
			// ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//Clearing for the UI
			glClear(GL_COLOR_BUFFER_BIT);

			Inputs::Process(window);
			Gui::Update();

			Globals::uTime += Globals::deltaTime;


			//Drawing shapes
			Camera::ActiveCamera->Capture();
			//Update Scripts
			for (auto it = Script::componentList.begin(); it != Script::componentList.end(); it++) {
				if (it->second.script != nullptr) it->second.script->Update();
			}
			Camera::ActiveCamera->Update();

			//Render Im::Gui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//Update screen
			glfwSwapBuffers(window);
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
		Shutdown();
}
	

void NWengine::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
}

