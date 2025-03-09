#include "Scripts.h"
#include "Scene.h"
#include "DefaultAssets.h"
#include "Inputs.h"
#include "NWTime.h"
#include "Asset.h"
#include "ScriptingComp.h"
#include "Keyboard.h"
#include "RenderingPipeline.h"

#define ADD_OBJ(scenePtr,name, variable) GameObject& variable =  s->AddObject(); s->Rename(name, &variable)

DynamicScript* dynScr; 
void bg(int offset) {
	Scene* s = Scene::GetCurrent();
	ADD_OBJ(s, "Bg0", bg);
	auto spr = bg.AddComponent<Sprite>();
	auto tr = bg.AddComponent<Transform>();
	spr->SetShader(ShaderTexturedDefaultStr, &ShaderTexturedDefaultID);
	spr->SetTexture("../Sandbox/assets/Images/bg.png");
	spr->SetSortingLayer(BG_LAYER);
	tr->scale.y = 2.0;
	tr->scale.x = 2.0;
	tr->position.y += 70.0;
	tr->position.x += -500.0 + offset * tr->scale.x * spr->container.width;
}

void Background() {
	for (int i = 0; i < 10; ++i) {
		bg(i);
	}
}

inline bool cond(GameObject* obj) {
	return obj->name[0] == 'S';
}

static float tt =0.0; //TODO::DELETE
void GameManager::Start() {
	Scene* s = Scene::GetCurrent();
	s->SetAutoCache(1);
	s->AddToCache(cond);
	
	ADD_OBJ(s, "CameraObj",   cam);
	ADD_OBJ(s, "WaterObj",	  water);
	ADD_OBJ(s, "GroundObj",   ground);
	ADD_OBJ(s, "TreeObj",     tree);
	ADD_OBJ(s, "TextObj",     text);
	ADD_OBJ(s, "SpawnerObj",  spawner);
    ADD_OBJ(s, "Circle0", c0);
    ADD_OBJ(s, "Circle1", c1);

	cam.AddComponent<Script>()->SetScript<CameraScript>();
	water.AddComponent<Script>()->SetScript<WaterScript>();
	ground.AddComponent<Transform>();
	auto spr = ground.AddComponent<Sprite>();
	auto tr  = ground.AddComponent<Transform>();	
	
	//Adds ground and the tree---
	spr->SetShader(NW_DEFAULT_SHADER);
	spr->SetTexture("../Sandbox/assets/Images/gnd.png");
	spr->SetSortingLayer(GROUND_LAYER);
	tr->scale = fVec2(1.3, 1.3);
	tr->position.y  = -56.0;

    tr = c0.AddComponent<Transform>();
    spr=c0.AddComponent<Sprite>();
    spr->SetSortingLayer(GROUND_LAYER);
	spr->SetTexture(NW_DEFAULT_TEXTURE);
    spr->SetShader("../Sandbox/assets/Shaders/Circle.shader");
    tr->Translate({-200.0,0.0});
    spr->container.UpdateSize(128, 128);

    tr =c1.AddComponent<Transform>();
    spr=c1.AddComponent<Sprite>();
    spr->SetSortingLayer(GROUND_LAYER);
	spr->SetTexture(NW_DEFAULT_TEXTURE);
    spr->SetShader("../Sandbox/assets/Shaders/Circle.shader");
    tr->Translate({16.0,0.0});
    tr->Translate({-200.0,0.0});


	spr  = tree.AddComponent<Sprite>();
	tr   = tree.AddComponent<Transform>();
	spr->SetShader("../Sandbox/assets/Shaders/Tree.shader");
	spr->SetTexture("../Sandbox/assets/Images/tree.png");
	spr->SetSortingLayer(TREE_LAYER);
	tr->scale = fVec2(2.0, 2.0);
	tr->position.y += 39.0;
	tr->position.x += 270.0;

	//Adds background---
	Background();
	
	//Adds text
	auto te = text.AddComponent<Text>();

    te->SetShader(ShaderTextBatchedStr, &ShaderTextBatchedDefaultID);
	te->SetFont({"../Sandbox/assets/Fonts/Cloudy.ttf",0},te->_shader);

	te->text = "NWengine 0.9!";
	te->layerOrder = TEXT_LAYER;
	te->colors       = fVec4(0.94,0.95, 1.0, 1.0);
	te->SetScale(fVec2(1.0,1.0));
	te->UpdateGlyphs();
    te->SetPosition(fVec2(-te->GetSize().x * 0.5f, 130.0f));
	te->UpdateGlyphs();
	
    te->SetChrCallback([](Character* chr, TextIterData* tdata){
        Transform* tr = chr->go.GetComponent<Transform>();
        tr->position.y += 8.0 * cos(2.0*PI*0.1*tt + 2.0*PI * tdata->chrIndex/tdata->chrNum);
    });

	//Adds the leaves spawner
	spawner.AddComponent<Script>()->SetScript<Spawner>();
}

static float t = 0.0;
extern NWPPFX::Bloom bloomTst;
extern NWPPFX::Tonemapper tm;
extern NWPPFX::ColorCorrection cc;

void GameManager::Update() {
	t += NWTime::GetDeltaTime();
    GameObject* c = Scene::currentScene->GetGameObject("Circle0");
    GameObject* c1 = Scene::currentScene->GetGameObject("Circle1");
    Transform* tr = c->GetComponent<Transform>();
    CircleCollider* col0 = c->AddComponent<CircleCollider>();
    CircleCollider* col1 = c1->AddComponent<CircleCollider>();
    col0->SetRadius(64.0); col1->SetRadius(8.0);
    tr->Translate(fVec2(Inputs::right - Inputs::left, Inputs::up - Inputs::down).normalize() * 200.0 * NWTime::GetDeltaTime());
    fVec2 depthBuff = fVec2(0.0,0.0);
    col0->isColliding(col1, &depthBuff);
    tr->Translate(depthBuff);
    depthBuff = {0.0,0.0};
    tr->Translate(depthBuff);
    if (Inputs::GetInputKey('X', NWin::KeyEventEnum::NWIN_KeyReleased)) {
        //tm.spec.type = NWPPFX::TonemapperType::Reinhard;
        cc.spec.brightness += 0.03;
        cc.spec.contrast -= 0.1;
    }
    if (Inputs::GetInputKey('C', NWin::KeyEventEnum::NWIN_KeyReleased)) {
        //tm.spec.type = NWPPFX::TonemapperType::Uncharted;
        cc.spec.brightness -= 0.03;
        cc.spec.contrast += 0.1;
    }
    if (Inputs::GetInputKey('V', NWin::KeyEventEnum::NWIN_KeyReleased)) {
        tm.spec.type = NWPPFX::TonemapperType::ACESNrkz;
    }
    if (Inputs::GetInputKey('B', NWin::KeyEventEnum::NWIN_KeyReleased)) {
        tm.spec.type = NWPPFX::TonemapperType::Uncharted;
    }
	auto tree = Scene::GetCurrent()->GetGameObject("TreeObj");
	tree->GetComponent<Sprite>()->shader->Use();
	tree->GetComponent<Sprite>()->shader->SetUniform1f("uTime", t);
	tree->GetComponent<Sprite>()->shader->Unuse();

    tt += NWTime::GetDeltaTime();
}
