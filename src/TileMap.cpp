#include "TileMap.h"
#include "Camera.h"
#include "RessourcesLoader.h"
#include "Inputs.h"

#include <fstream>
#include <string>
#include <cstring>

extern Camera camera;
static GameObjectClone til = GameObjectClone(wallTile0);

TileMap::TileMap(const char* name,Vector2<int> cellSize, Vector3<float> color)
{
	this->name = _strdup(name);
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
	SetUpTiles();
}
TileMap::~TileMap() {
	std::ofstream tilemapData((std::string)"Maps/" + this->name + (std::string) ".txt");
	for (auto it = tiles.begin(); it != tiles.end(); it++) {
		tilemapData << it->originalGameObject->id << " "
					<< it->position.x << " " << it->position.y << " " 
					<< it->scale.x << " "<<it->scale.y << "\n";

	}
	tilemapData.close();

}

void TileMap::RenderGrid(){
	for (Line line : grid) {
		line.Draw();
	}
}
void TileMap::SetUpTiles() {
	std::ifstream file("Maps/" + (std::string)name + std::string(".txt"));
	if (file) {
		for (std::string line; std::getline(file, line);)
		{
			if (line[0] == ' ' || !line[0] || line[0] == '\n') break;
			std::string arr[5] = {""};
			uint8_t current = 0;
			for (int i = 0; i < line.size(); i++) {
				char letter = line[i];
				if (letter == ' ') {
					current++;
					continue;
				}
				else if ( letter == '\n') break;
				arr[current] += letter;
			}

			til = GameObjectClone( allObjects[std::stoi(arr[0])] );
			til.position = Vector2<int>(std::stoi(arr[1]), std::stoi(arr[2]));
			til.scale = Vector2<float>(std::stof(arr[3]), std::stof(arr[4]));
			tiles.push_back(til);
		}
	}
	else std::cout <<("Maps/" + (std::string)name + std::string(".txt"));
}
void TileMap::Update() {
	m_canTile -= deltaTime;
	signed char a = cellSize.x / 2 ;
	signed char b = cellSize.y / 2 ;
	if (m_canTile < 0) {

		int X = camera.position.x + mousePosX - SCREEN_WIDTH / 2;
		int Y = camera.position.y - mousePosY + SCREEN_HEIGHT / 2;
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
				til.scale = Vector2<float>(1.0f, 1.0f);
				tiles.push_back(til);
				m_canTile = canTile;
			}
		}
		if (input_0) til = GameObjectClone(groundTile0);
		else if (input_1) til = GameObjectClone(groundTile1);
		else if (input_2) til = GameObjectClone(groundTile2);
		else if (input_3) til = GameObjectClone(wallTile0);
		
		else if(input_d && canTile && !tiles.empty()) {
			tiles.pop_back();
			m_canTile = canTile;
		}
	}
}