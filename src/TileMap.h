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
	float canTile = 0.1f; //Tiling frequency in seconds
private:
	float m_canTile = canTile;
	Line grid[1000];
	Vector2<int> cellSize;
	
};