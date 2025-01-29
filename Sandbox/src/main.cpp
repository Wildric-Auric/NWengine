#include "NWengine.h"
#include "Scripts.h"	
#include "Scene.h"
#include "DefaultAssets.h"
#include "windows.h"

Renderer CRT;

void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& manager = s.AddObject();
	s.Rename("GameManagerObj", &manager);
	manager.AddComponent<Script>()->SetScript<GameManager>();
	manager.AddComponent<MultiAudioEmitter>()->AddEmitter();
	manager.DeleteComponent<MultiAudioEmitter>();

	CRT.SetUp();
	//CRT.SetShader("../Sandbox/assets/Shaders/MattiasCRT.shader");
	CRT.Use();
	CRT.stretchCoeff.x = 1.3;
	CRT.stretchCoeff.y = 1.3;
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static float t = 0.0;

void Render() {
	t += NWTime::GetDeltaTime();
	Renderer& renderer = (*Renderer::currentRenderer);
	Shader::resList;
	renderer.componentContainer.GetComponent<Sprite>()->shader->Use();
    renderer.componentContainer.GetComponent<Sprite>()->shader->SetUniform1f("uTime", t);
	renderer.componentContainer.GetComponent<Sprite>()->shader->Unuse();

	renderer(true);	
}

int main() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;

	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
	return 0;
}

