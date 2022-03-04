#pragma once
#include "GameObject.h"
#include<vector>
#include<deque>

class Scene {
public:
	const char* name;
	std::deque<GameObjectClone> sceneObjs;
	std::vector<GameObjectClone*> drawList;
	void AddObject(GameObjectClone goc);
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