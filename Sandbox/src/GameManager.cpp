#include "Scripts.h"
#include "Scene.h"
#include "Components.h"
#include "DefaultAssets.h"
#include "Inputs.h"
#include "NWTime.h"
#include "Asset.h"
#include "DllScripting.h"
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
	ADD_OBJ(s, "Square", sq);
    ADD_OBJ(s, "Circle0", c0);
    ADD_OBJ(s, "Circle1", c1);

	cam.AddComponent<Script>()->SetScript<CameraScript>();
	water.AddComponent<Script>()->SetScript<WaterScript>();
	ground.AddComponent<Transform>();
	auto spr = ground.AddComponent<Sprite>();
	auto tr  = ground.AddComponent<Transform>();	
	
	//Adds ground and the tree---
	spr->SetShader(ShaderTexturedDefaultStr, &ShaderTexturedDefaultID);
	spr->SetTexture("../Sandbox/assets/Images/gnd.png");
	spr->SetSortingLayer(GROUND_LAYER);
	tr->scale = fVec2(1.3, 1.3);
	tr->position.y  = -56.0;

    tr = c0.AddComponent<Transform>();
    spr=c0.AddComponent<Sprite>();
    spr->SetSortingLayer(GROUND_LAYER);
	spr->SetTexture(&ImageDefault, &TextureDefaultID);
    spr->SetShader("../Sandbox/assets/Shaders/Circle.shader");
    tr->Translate({-200.0,0.0});
    spr->container.UpdateSize(128, 128);

    tr =c1.AddComponent<Transform>();
    spr=c1.AddComponent<Sprite>();
    spr->SetSortingLayer(GROUND_LAYER);
	spr->SetTexture(&ImageDefault, &TextureDefaultID);
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
	Loader<Shader> l;
	te->SetFont("../Sandbox/assets/Fonts/Cloudy.ttf",
	l.LoadFromBufferOrGetFromCache(&ShaderTextBatchedDefaultID, (void*)&ShaderTextBatchedStr, &ShaderTextBatchedDefaultID));
	
	te->text = "NWengine 0.9!";
	te->layerOrder = TEXT_LAYER;
	te->colors       = fVec4(0.94,0.95, 1.0, 1.0);
	te->scale        = fVec2(.8, 0.8);
	te->UpdateGlyphs();
	te->position.x -= te->GetSize().x * 0.5;
	te->position.y += 130.0;
	te->UpdateGlyphs();
	
	//Adds the leaves spawner
	spawner.AddComponent<Script>()->SetScript<Spawner>();

	tr = sq.AddComponent<Transform>();
	spr = sq.AddComponent<Sprite>();
	spr->SetShader(ShaderTexturedDefaultStr, &ShaderTexturedDefaultID);
	spr->SetTexture(&ImageDefault,&TextureDefaultID);
	tr->scale = tr->scale * 5.0;
    sq.AddComponent<Collider>();
    //spr->StopRendering();
	//dynamic Script debugging
	DllScripting::CompileDll({ "../Sandbox/src/DynScriptTest.cpp" }, "DllPath/test.dll");
	dynScr = sq.AddComponent<DynamicScript>();	
	dynScr->Load("DllPath/test.dll");
	
	//dynScr->Unload();
	//dynScr->Load("DllPath/test.dll");
	const char* (*f)()  = (const char* (*)())dynScr->dllScript->GetDllFunc("helloWorld");
	printf("%s\n", f());
}

static float t = 0.0;

extern NWPPFX::Bloom bloomTst;

void GameManager::Update() {
    GameObject* sq = Scene::currentScene->GetGameObject("Square");
    sq->GetComponent<Collider>()->SetEdgesSprite();

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
    Collider* sqCol = sq->GetComponent<Collider>();
    col0->isColliding(sqCol, &depthBuff);
    tr->Translate(depthBuff);

    if (Inputs::GetInputKey('X', NWin::KeyEventEnum::NWIN_KeyReleased)) {
//        Camera::GetActiveCamera()->ChangeOrtho(Camera::ActiveCamera->size.x * 2.0, 
//                Camera::ActiveCamera->size.y * 2.0);
        //tm.whitePoint -= 0.1;
        bloomTst.luminanceThreshold -= 0.5;
    }

    if (Inputs::GetInputKey('C', NWin::KeyEventEnum::NWIN_KeyReleased)) {
//        Camera::GetActiveCamera()->ChangeOrtho(Camera::ActiveCamera->size.x * 0.5, 
//                Camera::ActiveCamera->size.y * 0.5);
        //tm.whitePoint += 0.1;
        bloomTst.luminanceThreshold += 0.5;
    }


	float hw = Context::WINDOW_WIDTH  * 0.5f;
	float hh = Context::WINDOW_HEIGHT * 0.5f;
	auto tree = Scene::GetCurrent()->GetGameObject("TreeObj");
	//tree->GetComponent<Transform>()->position = fVec2(Inputs::mousePosX - hw, -Inputs::mousePosY + hh);
	tree->GetComponent<Sprite>()->shader->Use();
	tree->GetComponent<Sprite>()->shader->SetUniform1f("uTime", t);
	tree->GetComponent<Sprite>()->shader->Unuse();
	static float a = 5.0;
	if (Inputs::GetInputKey('D', NWin::KeyEventEnum::NWIN_KeyReleased)) {
		dynScr->Unload();
		DllScripting::CompileDll({ "../Sandbox/src/DynScriptTest.cpp" }, "DllPath/test.dll");
		dynScr->Reload("DllPath/test.dll");
	}

	float(*f)()  = (float(*)())dynScr->dllScript->GetDllFunc("getFloat");
	a = f();
	Scene::GetCurrent()->GetGameObject("Square")->GetComponent<Transform>()->rotation = t * a;
}
