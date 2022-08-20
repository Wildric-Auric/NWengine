#pragma once
#include "GameObject.h"
#include<list>

class Scene {
public:
	const char* name;
	std::list<GameObject> sceneObjs;
	std::list<GameObject*> drawList;
	void AddObject(GameObject goc);
	void DeleteObject(uint32 index);
	void DeleteObject(std::string name);
	GameObject* GetGameObject(std::string name);
	void SortScene();
	Scene(const char* name);
	~Scene();
	void LoadScene();
	void Update();
	void Draw();
	void Save();
	static bool GuiActive;
	static Scene* currentScene;
	static void Gui();
};