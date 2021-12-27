#include "TileMap.h"
#include "Camera.h"
#include "RessourcesLoader.h"
#include "Inputs.h"
#include <iostream>
extern Camera camera;
TileMap::TileMap(Vector2<int> cellSize, Vector3<float> color) 
{
	this->cellSize = cellSize;
	int lineNum = 1000;
	int half = lineNum / 2;
	int quarter = lineNum / 4;
	signed char a = cellSize.x / 2*0;
	signed char b = cellSize.y / 2*0;
	for (int i = 0; i < half; i++) {
		grid[i] = Line(Vector2<int>(-1000, i * cellSize.x- quarter*cellSize.x - b), Vector2<int>(1000, i * cellSize.x - quarter * cellSize.x -b), color);
	}
	for (int j = 0; j < half; j++) {
		grid[j + half] = Line(Vector2<int>(j * cellSize.y - quarter * cellSize.y -  a, -1000), Vector2<int>(j * cellSize.y - quarter * cellSize.y - a, 1000), color);
	}
}

void TileMap::RenderGrid(){
	for (Line line : grid) {
		line.Draw();
	}
}

GameObjectClone til = GameObjectClone(tile1Obj);
void TileMap::Update() {
	m_canTile -= deltaTime;
	signed char a = cellSize.x / 2 ;
	signed char b = cellSize.y / 2 ;
	if (m_canTile < 0) {

		int X = camera.position.x + mousePosX - SCREEN_WIDTH / 2 + 1;
		int Y = camera.position.y - mousePosY + SCREEN_HEIGHT / 2 +1;
		if (input_left_click) {
			bool bo = false;

			Vector2<int> pos = Vector2<int>((X / cellSize.x - (X<0)) * cellSize.x + a, (Y / cellSize.y - (Y < 0)) * cellSize.y + b);
			for (auto it = tiles.begin(); it != tiles.end(); it++) {
				if (it->position == pos) {
					bo = true;
					break;
				}
			}
			if (!bo) {
				til.position = pos;
				tiles.push_back(til);
				m_canTile = canTile;
			}
		}
		else if(input_d && canTile && !tiles.empty()) {
			tiles.pop_back();
			m_canTile = canTile;
		}
	}
}