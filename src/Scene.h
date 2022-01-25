#pragma once
#include "GameObject.h"
#include<vector>
class Scene {
public:
	const char* name;
	std::vector<GameObjectClone> sceneObjs;
	Scene(const char* name);
	~Scene();
	void LoadScene();
	void Update();
	void Draw();
	void Save();
	static bool GuiActive;
	static Scene* currentTileMap;
	static void Gui();
};