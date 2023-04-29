
#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include<list>

class Scene {
public:
	std::string name;
	std::list<GameObject> sceneObjs;
	std::list<Sprite*> drawList;
	void AddObject(GameObject goc);
	void DeleteObject(uint32 index);
	void DeleteObject(std::string name);
	GameObject* GetGameObject(std::string name);
	void SortScene();
	void Render(Sprite* sprite);
	void ForceRenderStop();
	void Rearrange(Sprite* sprite);
	Scene(std::string name);
	~Scene();
	void LoadScene();
	void Start();
	void Update();
	void SetUp(); //Sets up needed static variables and other things
	void Draw();
	void Save();
	static bool GuiActive;
	static Scene* currentScene;
	static void Gui();
};