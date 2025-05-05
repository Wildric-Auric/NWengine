#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"

namespace TextTst {

static Text*	  text;
static Transform* box;
static fVec2	  direction = fVec2(1.0, -1.0).normalize();

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam		= s.AddObject();
	GameObject& str		= s.AddObject();
	GameObject& textBox = s.AddObject();
	Camera*		camC	= cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);
	Text* te = str.AddComponent<Text>();
	// te->SetPosition({200.0,-100.0});
	te->SetFont({"../Sandbox/assets/Fonts/cloudy.ttf", 32}, NW_DEFAULT_SHADER_TEXT_BATCHED);
	std::string ifont;
	GetSystemFontDir(&ifont);
	//    ifont += "arial.ttf";
	//    te->SetScale(fVec2(1.0,1.0));
	//    te->SetFont({ifont.c_str(), 32}, NW_DEFAULT_SHADER_TEXT_BATCHED);
	te->SetBoxHorizontalWrap(180.0f);

	te->SetContentAndUpdateGlyphs("Hello jiji");

	te->colors = fVec4(1.0, 0.0, 1.0, 1.0);

	GameObject& bg		= s.AddObject();
	bg.AddComponent<Transform>();
	Sprite* spr = bg.AddComponent<Sprite>();
	spr->SetTexture(NW_DEFAULT_TEXTURE);
	spr->SetShader("../Sandbox/src/templates/Blueprint.shader");
	spr->SetSize(camC->GetSize());
	spr->GetShader()->Use();
	spr->GetShader()->SetVector2("uResolution", spr->container.width, spr->container.height);
	spr->GetShader()->SetVector2("uCell", 50.0f, 50.0f);
	spr->SetSortingLayer(100);

	Transform* tr = textBox.AddComponent<Transform>();
	fVec2	   st = te->GetSize();
	// tr->SetPosition(te->GetPosition() + fVec2(st.x /2.0, st.y / 2.0 - tmp0));
	spr = textBox.AddComponent<Sprite>();
	spr->SetTexture(NW_DEFAULT_TEXTURE);
	spr->SetShader(NW_DEFAULT_SHADER);
	spr->SetSortingLayer(10);
	spr->SetSize(te->GetBBRef().size);
	printf("%d %d\n", spr->container.width, spr->container.height);
	tr->SetPosition(te->GetBBRef().center);

	text = te;
	box	 = tr;
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
	(*Renderer::defaultRenderer)(true);

	float mmag	 = 100.0;
	fVec2 newPos = text->GetPosition() + direction * mmag * NWTime::GetDeltaTime();
	fVec2 bsize	 = text->GetSize();
	fVec2 csize	 = Camera::GetActiveCamera()->GetSize();
	if(newPos.x + bsize.x * 0.5 >= csize.x * 0.5 || newPos.x - bsize.x * 0.5 <= -csize.x * 0.5) {
		direction.x = -direction.x;
	}
	if(newPos.y + bsize.y * 0.5 >= csize.y * 0.5 || newPos.y - bsize.y * 0.5 <= -csize.y * 0.5) {
		direction.y = -direction.y;
	}
	text->SetPosition(text->GetPosition() + direction * mmag * NWTime::GetDeltaTime());
	text->UpdateGlyphs();
	box->SetPosition(text->GetPosition());
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
}; // namespace TextTst
