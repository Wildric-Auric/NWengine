
#include "TileMap.h"
#include "Camera.h"
#include "RessourcesLoader.h"
#include "Inputs.h"

#include <fstream>
#include <string>
#include <cstring>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Globals.h"

#include<iostream>

extern Camera camera;
static GameObjectClone currentTile;

TileMap::TileMap(const char* name,Vector2<int> cellSize, Vector3<float> color)
{
	currentTile = GameObjectClone(&objects["wallTile0"]); //Handles exception

	TileMap::currentTileMap = this;
	this->name = _strdup(name);
	this->cellSize = cellSize;
	int lineNum = 1000;
	int half = lineNum / 2;
	int quarter = lineNum / 4;
	signed char a = cellSize.x / 2*0;
	signed char b = cellSize.y / 2*0;
	//for (int i = 0; i < half; i++) {
	//	grid[i] = Line(Vector2<int>(-1000, i * cellSize.x- quarter*cellSize.x - b), Vector2<int>(1000, i * cellSize.x - quarter * cellSize.x -b), color);
	//}
	//for (int j = 0; j < half; j++) {
	//	grid[j + half] = Line(Vector2<int>(j * cellSize.y - quarter * cellSize.y -  a, -1000), Vector2<int>(j * cellSize.y - quarter * cellSize.y - a, 1000), color);
	//}
	SetUpTiles();
}
TileMap::~TileMap() {
	std::ofstream tilemapData((std::string)"Maps/" + this->name + (std::string) ".txt");
	for (auto it = tiles.begin(); it != tiles.end(); it++) {
		tilemapData << it->originalGameObject->name << " "
					<< it->position.x << " " << it->position.y << " " 
					<< it->scale.x << " "<<it->scale.y << "\n";

	}
	tilemapData.close();

}

void TileMap::RenderGrid(){
	//for (Line line : grid) {
	//	line.Draw();
	//}
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

			currentTile = GameObjectClone( &objects[arr[0]] );
			currentTile.position = Vector2<int>(std::stoi(arr[1]), std::stoi(arr[2]));
			currentTile.scale = Vector2<float>(std::stof(arr[3]), std::stof(arr[4]));
			tiles.push_back(currentTile);
		}
	}
	else std::cout <<("Maps/" + (std::string)name + std::string(".txt"));
}

//TODO::Working tilling system
extern int  p1;
extern int p2;
void TileMap::Update() {

	if (GuiActive) {
		m_canTile -= deltaTime;
		signed char a = cellSize.x / 2 ;
		signed char b = cellSize.y / 2 ;
		if (m_canTile < 0 && isMouseOnGui) {

			int X = camera.position.x + mousePosX - p1 - RENDERING_WIDTH /2;
			int Y = camera.position.y + mousePosY - WINDOW_HEIGHT + p2 + RENDERING_HEIGHT/2;		//Suffering...
			if (input_left_click) {
				bool bo = false;
				Vector2<int> pos = Vector2<int>((X / cellSize.x - (X < 0)) * cellSize.x + a, (Y / cellSize.y - (Y < 0)) * cellSize.y + b);
				for (auto it = tiles.begin(); it != tiles.end(); it++) {
					if (it->position == pos) {
						bo = true;
						break;
					}
				}
				if (!bo) {
					currentTile.position = pos;
					currentTile.scale = Vector2<float>(1.0f, 1.0f);
					tiles.push_back(currentTile);
					m_canTile = canTile;
				}
			}

			else if (input_d && canTile && !tiles.empty()) {
				tiles.pop_back();
				m_canTile = canTile;
			}
		}
	}
}

bool TileMap::GuiActive = false;
TileMap* TileMap::currentTileMap;

void TileMap::Gui() {
	if (TileMap::GuiActive) {
		ImGui::Begin("Tiles editor", 0, ImGuiWindowFlags_MenuBar);
		if (ImGui::Button("Delete") && currentTileMap->canTile && !currentTileMap->tiles.empty()) {
			currentTileMap->tiles.pop_back();
			int i = 0;
			for (auto it = currentTileMap->tiles.begin(); it != currentTileMap->tiles.end(); it++) {
				i += 1;
			}
			currentTileMap->m_canTile = currentTileMap->canTile;
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close", "Ctrl+W")) TileMap::GuiActive = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();

		}

		for (int i = 0; i < 10; i++) {
			GameObject* temp = currentTileMap->tileObjects[i];
			if (temp != nullptr) {
				ImGui::Image((void*)(intptr_t)
					temp->image->texture,
					ImVec2(temp->image->size.x, temp->image->size.y), ImVec2(0, 1), ImVec2(1, 0));
				std::string str = "Tile" + std::to_string(i);
				if (ImGui::Button(str.c_str())) {
					currentTile = GameObjectClone(temp);
				};
			}
		}
		ImGui::End();
	};

}

void TileMap::Draw() {
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		(*it).Draw(0);
	}
}

