#include "NWengine.h"

#define NW_CALL_EX(locatinon) for (void(*func)() : functionMap[locatinon]) func();

/*Preprocessor Macro should as defined as follows :
#define NW_VERSION "0.9.291224"
*/
#ifdef NW_VERSION
const char* NWVersionString = NW_VERSION;
#else
const char* NWVersionString = "";
#endif


NW_PREFIX const char* NWengineGetVersionString() {
    return NWVersionString;
}

NW_PREFIX void  NWengineGetVersion(NWengineVersion* outVers) {
    const char* versionStr = NWengineGetVersionString(); 
    const char* cur = versionStr;
    if (cur == '\0') return;

    std::string temp = "";
    while (*cur != '.') {
        temp += *cur; 
        ++cur;
    }
    outVers->major = std::stoi(temp);
    ++cur;

    temp = "";
    while (*cur != '.') {
        temp += *cur; 
        ++cur;
    }
    outVers->minor = std::stoi(temp);
    ++cur;

    temp = *cur; ++cur;
    temp += *cur; ++cur;
    outVers->day = std::stoi(temp);

	temp = *cur; ++cur;
	temp += *cur; ++cur;
    outVers->month = std::stoi(temp);

	temp = *cur; ++cur;
	temp += *cur; ++cur;
    outVers->year = std::stoi(temp);
}

std::unordered_map<ON_MAIN_CALL_LOCATION, std::vector<void(*)()>> functionMap;

NW_PREFIX int NWengineInit() {
	//Opens a window and init openGL context
	void* window = Context::InitContext(Context::WINDOW_WIDTH, Context::WINDOW_HEIGHT);
	if (window == nullptr) return -1;
	//Init Timer, absolute 0
	NWTime::Init();
	NW_CALL_EX(ON_MAIN_CALL_LOCATION::InitBegin)
	
	//init InitSoundSystem
	if (!Sound::Init()) { NW_LOG_ERROR("Could not init audio engine");return -1; }
	//init Text System
	if (!Font::Init())
		return -1;
	//Context settings
	Context::EnableBlend();
    
	Primitives::Init();
    Batch::Init();
	Renderer::Init();

	NW_CALL_EX(ON_MAIN_CALL_LOCATION::InitEnd)

	return 0;
}

NW_PREFIX void NWengineLoop() {
	while (!Context::ShouldClose()) {
		NWTime::Init();
		//Initializing imgui here for example
		NW_CALL_EX(ON_MAIN_CALL_LOCATION::FrameBegin)
		//Clearing the context
		Context::Clear();
		Camera::UpdateActiveCamera();
		Scene::UpdateActiveScene();
		//Any rendering should be done here
		NW_CALL_EX(ON_MAIN_CALL_LOCATION::FrameIntermediate)
		//Updating imgui for example
		NW_CALL_EX(ON_MAIN_CALL_LOCATION::FrameEnd)


		Inputs::Process(Context::window);
		Context::Update();

		
		//Calculate fps
		NWTime::Update();
	}
}

NW_PREFIX void NWengineShutdown() {
	NW_CALL_EX(ON_MAIN_CALL_LOCATION::Destroy)
	Scene::Destroy();
    Asset::Destroy();
	Renderer::Destroy();
	Primitives::Destroy();
    Batch::Destroy();
	Context::Destroy();
	Font::Destroy();
	Sound::Destroy();
}

NW_PREFIX bool NWenginePushFunction(ON_MAIN_CALL_LOCATION loc, void(*func)()) {
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

NW_PREFIX bool NWenginePopFunction(ON_MAIN_CALL_LOCATION loc, void(*func)()) {
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
