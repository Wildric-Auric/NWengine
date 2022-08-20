#include "NWengine.h"
#include "Animation.h"
#include "Animator.h"


int8 NWengine::Run() {


		GLFWwindow* window = Context::InitContext(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT);
		if (window == nullptr) return -1;

		//init imgui
		Gui::Init((int)window);
		//init OpenAL
		if (!InitOpenAL()) return -1;

		//Init scripting (native)
		NW::ptr = new NW;
		*NW::ptr = NW();
		NW::ptr->Start();
		//Load ressources
		RessourcesLoader::LoadDefaultRessources();

		Context::EnableBlend();

		////Initialization finished
		//TODO::UI for scene load ans serialization
		Scene scene0 = Scene("scene0");
		scene0.LoadScene();
		
		MainLoop();

		Shutdown();
}

//------------------
void NWengine::MainLoop() {

	int frameCount = 0;
	double currentTime;
	double lastTime;
	double deltaTimeSum = 0;
	lastTime = glfwGetTime();

	//TESTING----------------

	//GameObject pp = GameObject();
	//Sprite* ppspr = pp.AddComponent<Sprite>();
	//ppspr->SetShader("Shaders/Test.shader");
	//ppspr->container = Quad(iVec2(0, 0), Globals::NATIVE_WIDTH, Globals::NATIVE_HEIGHT);
	//ppspr->texture = Camera::ActiveCamera->fbo.RenderedImage;
	//Animation test
	//Animation anima = Animation();
	//anima.frames.push_back(RessourcesLoader::LoadTexture("Ressources\\Images\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_1.png"));
	//anima.frames.push_back(RessourcesLoader::LoadTexture("Ressources\\Images\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_2.png"));
	//anima.frames.push_back(RessourcesLoader::LoadTexture("Ressources\\Images\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_3.png"));
	//anima.frames.push_back(RessourcesLoader::LoadTexture("Ressources\\Images\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_4.png"));
	//anima.frames.push_back(RessourcesLoader::LoadTexture("Ressources\\Images\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_5.png"));
	//anima.frames.push_back(RessourcesLoader::LoadTexture("Ressources\\Images\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_6.png"));

	//for (int i = 0; i < 6; i++)
	//	anima.durations.push_back(0.16);
	//GameObject* obj = Scene::currentScene->GetGameObject("Warrior");
	//obj->AddComponent<Animator>()->animation = anima;
	
	//-------------------------

	while (!glfwWindowShouldClose(Context::window)) {
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Clearing for the UI
		Context::Clear();

		Inputs::Process(Context::window);
		Gui::Update();

		Globals::uTime += Globals::deltaTime;

		//Drawing shapes
		Camera::ActiveCamera->Capture(0.0, 0.0,0.0);
		//Update scripting interface
		NW::ptr->Update();
	    
		Camera::ActiveCamera->Update();
		Scene::currentScene->Update();

		//Render Im::Gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//----------------------------------

		//Context::Clear(0.3f,0.0f,1.0f);
		//pp.Draw();

		//----------------------------------

		//Update screen
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


void NWengine::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
}

