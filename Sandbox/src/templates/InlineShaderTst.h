
#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"
#include "InlineShader.h"


namespace InlineShaderTst {

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
    GameObject& cam = s.AddObject();
    Camera* camC = cam.AddComponent<Camera>();
    camC->Use();
    camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
    camC->ChangeOrtho(720,480);
    auto& obj = s.AddObject();
    obj.AddComponent<Transform>();
    Sprite* spr = obj.AddComponent<Sprite>();
    InlineShader ish;
    ish.AppFragGlobal("uniform vec3 uColor;");
    ish.AppFragMain("vec3 c = uColor;");
    ish.SetFragOut("vec4(c,1.0)");
    ish.Generate();
    spr->SetShader(ish._shader);
    spr->shader->Use();
    spr->shader->SetUniform3f("uColor",1.0,0.0,0.0);
    s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
    (*Renderer::defaultRenderer)(true);
}

void Run() {
        Context::_glInfo.maxVersion = 4;
        Context::_glInfo.minVersion = 6;
        NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
        NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
        NWengineInit();
        NWengineLoop();
        NWengineShutdown();
    }
};

