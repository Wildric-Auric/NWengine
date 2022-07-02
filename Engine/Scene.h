#pragma once
#include "GameObject.h"
#include<vector>
#include<deque>

class Scene {
public:
	const char* name;
	std::deque<GameObject> sceneObjs;
	std::vector<GameObject*> drawList;
	void AddObject(GameObject goc);
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