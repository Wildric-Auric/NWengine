#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"


namespace TextTst {

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
    GameObject& cam = s.AddObject();
    GameObject& str = s.AddObject();
    GameObject& textBox = s.AddObject();
    GameObject& bg  = s.AddObject();
    Camera* camC = cam.AddComponent<Camera>();
    camC->Use();
    camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
    camC->ChangeOrtho(720,480); 
    Text* te = str.AddComponent<Text>();
    te->SetFont({"../Sandbox/assets/Fonts/cloudy.ttf", 32}, NW_DEFAULT_SHADER_TEXT_BATCHED);
    //te->SetFont({"C:/Programming/artha/ImageProcessing/Visualizer/example/assets/Fonts/arial.ttf", 64}, NW_DEFAULT_SHADER_TEXT_BATCHED);
    te->SetBoxHorizontalWrap(180.0f);

    te->SetContentAndUpdateGlyphs("Hjjlo World hjljej");

    te->colors = fVec4(1.0,.0,.0,1.0);
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
    fVec2 st = te->GetSize();
    //tr->SetPosition(te->GetPosition() + fVec2(st.x /2.0, st.y / 2.0 - tmp0));
    spr = textBox.AddComponent<Sprite>();
    spr->SetTexture(NW_DEFAULT_TEXTURE);
    spr->SetShader(NW_DEFAULT_SHADER);
    spr->SetSortingLayer(10);
    spr->SetSize(te->GetBBRef().size);
    printf("%d %d\n", spr->container.width, spr->container.height);
    tr->SetPosition(te->GetBBRef().center);
    
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

