#pragma once
#include "Maths.h"
#include "Primitives.h"
#include "GameObject.h"
#include <vector>

class TileMap {
public:
	TileMap(const char* name, Vector2<int> cellSize = Vector2<int>(16, 16), Vector3<float> color = Vector3<float>(0.f, 0.0f, 0.0f));
	~TileMap();
    const char* name;
	std::vector<GameObjectClone> tiles;
	void SetUpTiles();
	void RenderGrid();
	void Update();
	GameObject* tileObjects[10] = { nullptr };

	static bool GuiActive;
	static TileMap* currentTileMap;
	static void Gui();
	float m_canTile = canTile;

	float canTile = 0.1f; //Tiling frequency in seconds
private:
	//Line grid[1000]; //TODO: make this static 
	//Commented because it puts too much data on stack memory
	Vector2<int> cellSize;
	
};