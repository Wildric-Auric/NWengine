#include "CircleRenderer.h"
#include "DefaultAssets.h"
#include "InlineShader.h"
#include "NWengine.h"
#include "Scene.h"
#include "UISys.h"
#include "Text.h"
#include "Renderer.h"
#include "Components.h"
#include "Inputs.h"
#include "UIWindow.h"
#include "NWTime.h"
#include "DefaultAssets.h"
#include "LineRenderer.h"
#include "NWengine.h"
#include "Scene.h"

namespace MSAATst {

static float t = 0.0;
static Transform* tr;
static bool msaaEnabled = 0;
Camera* camC;
static void	 Init() {
	 Context::SetTitle("Sandbox");
	 Context::EnableVSync();
	 Scene& s = Scene::CreateNew("New Scene");
	 s.MakeCurrent();
	 GameObject& cam  = s.AddObject();
	 camC = cam.AddComponent<Camera>();
	 camC->Use();
	 camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	 camC->ChangeOrtho(720, 480);
	 GameObject&   sq = s.AddObject();
	 tr			 = sq.AddComponent<Transform>();
	 Sprite* spr = sq.AddComponent<Sprite>();
	 spr->SetTexture(NW_DEFAULT_TEXTURE);
	 spr->SetShader(ShaderTexturedDefaultStr, &ShaderTexturedDefaultID);
	 spr->SetSize({200, 200});
	 s.Start();
	 printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
    if (Inputs::GetInputOnKeyRelease((keyN)'A')) {
        camC->ChangeOrthoFull(camC->GetSize().x, camC->GetSize().y, msaaEnabled ? MSAAValue::NW_MSx1 : NW_MSx8, camC->GetTexType());
        msaaEnabled = !msaaEnabled;
    }
	t += NWTime::GetDeltaTime() * 20.0;
	tr->SetRotation(t);
	(*Renderer::currentRenderer)(true);
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
}; // namespace LineTst
