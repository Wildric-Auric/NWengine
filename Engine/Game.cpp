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
	RenderedTexture->container = Quad(iVec2(0, 0), Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
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
		
		//FrameObject.GetComponent<Transform>()->scale = fVec2(RenderedTexture->texture->size.x / RenderedTexture->container.width, 
		//													 RenderedTexture->texture->size.y / RenderedTexture->container.height);
		
		Context::Clear(0.0, 0.0, 0.0, 1.0);
		FrameObject.GetComponent<Transform>()->scale = fVec2(1.0, 1.0);

		fVec2 scale = FrameObject.GetComponent<Transform>()->scale;
		FrameObject.GetComponent<Transform>()->position = iVec2(0, 0);

		cam->viewPortSize.x = Camera::ActiveCamera->fbo.RenderedImage.size.x * scale.x; 
		cam->viewPortSize.y = Camera::ActiveCamera->fbo.RenderedImage.size.y * scale.y;
		Context::SetViewPort(Max<int>(Context::WINDOW_WIDTH - cam->viewPortSize.x, 0)*0.5, Max<int>(Context::WINDOW_HEIGHT - cam->viewPortSize.y, 0)*0.5, cam->viewPortSize.x, cam->viewPortSize.y);
		cam->ChangeOrtho(cam->viewPortSize.x, cam->viewPortSize.y);

		//Context::SetViewPort(0, 0, , 800);
		//Context::SetViewPort(0, 0);
		Camera* temp = Camera::ActiveCamera;
		cam->Use();
		Camera::ActiveCamera->Update();
		FrameObject.Draw();
		temp->Use();
		
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

