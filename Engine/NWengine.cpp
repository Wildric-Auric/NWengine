#include "NWengine.h"

#define NW_CALL_EX(locatinon) for (void(*func)() : functionMap[locatinon]) func();


std::unordered_map<ON_MAIN_CALL_LOCATION, std::vector<void(*)()>> functionMap;
GameObject NWengineDefaultRendererContainer;


NW_PREFIX int NWengineInit() {
	void* window = Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
	if (window == nullptr) return -1;

	NW_CALL_EX(ON_MAIN_CALL_LOCATION::InitBegin)
	
	//init InitSoundSystem
	if (!SoundSystem::InitOpenAL()) return -1;
	//init Text System
	if (!TextSystem::Init())
		return -1;
	Primitives::Init();
	//Context settings
	Context::EnableBlend();
	Batch::ComputeIndices();
	Batch::maxBatchTextures = 32; //TODO::Make a function that uses glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS...
	//Init Renderer
	Renderer::defaultRenderer = new Renderer(&NWengineDefaultRendererContainer); //Unchanged by the user
	Renderer::currentRenderer = Renderer::defaultRenderer;

	NW_CALL_EX(ON_MAIN_CALL_LOCATION::InitEnd)

	return 0;
}

NW_NO_MANGLING void NWengineLoop() {
	while (!Context::ShouldClose()) {
		//Initializing imgui here for example
		NW_CALL_EX(ON_MAIN_CALL_LOCATION::FrameBegin)
		//Clearing the context
		Context::Clear();
		Inputs::Process(Context::window);
		Camera::UpdateActiveCamera();
		Scene::UpdateActiveScene();
		//Any rendering should be done here
		NW_CALL_EX(ON_MAIN_CALL_LOCATION::FrameIntermediate)

		//Update window and capturing inputs
		Context::Update();
	
		//Updating imgui for example
		NW_CALL_EX(ON_MAIN_CALL_LOCATION::FrameEnd)
		//Calculate fps
		NWTime::Update();
	}
}

NW_NO_MANGLING void NWengineShutdown() {
	NW_CALL_EX(ON_MAIN_CALL_LOCATION::Destroy)

	delete Scene::currentScene;
	delete Renderer::defaultRenderer;
	SoundSystem::DestroyOpenAL();
	TextSystem::Destroy();
	Primitives::Destroy();
	Context::Destroy();
}

NW_NO_MANGLING bool NWenginePushFunction(ON_MAIN_CALL_LOCATION loc, void(*func)()) {
	if ((void*)func == nullptr)
		return 0;
	std::vector<void(*)()>& vec = functionMap[loc];
	for (void(*e)() : vec) {
		if (e == func)
			return 0;
	}
	vec.push_back(func);
	return 1;
}

NW_NO_MANGLING bool NWenginePopFunction(ON_MAIN_CALL_LOCATION loc, void(*func)()) {
	auto pair = functionMap.find(loc);
	if (pair == functionMap.end() || pair->second.size() <= 0)
		return 0;
	if ((void*)func == nullptr) {
		pair->second.pop_back();
		return 1;
	}
	for (auto iter = pair->second.begin(); iter != pair->second.end(); iter++) {
		if (*iter == func) {
			pair->second.erase(iter);
			return 1; //Pay attention if you change this in the future you may get invalid access because we increment afterwards
		}
	}
	return 0;
}

#undef NW_CALL_EX
#undef NW_NO_MANGLING
#undef NW_EXPORT    
#undef NW_PREFIX