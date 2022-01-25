#include "Scene.h"
#include<string>
#include<fstream>
#include"GameObject.h"
#include"RessourcesLoader.h"

static GameObjectClone currentObj;

Scene::Scene(const char* name) {
	this->name = _strdup(name);
};

void Scene::LoadScene() {
	std::ifstream file("Scenes/" + (std::string)name + std::string(".txt"));
	if (file) {
		for (std::string line; std::getline(file, line);)
		{
			if (line[0] == ' ' || !line[0] || line[0] == '\n') break;
			std::string arr[5] = { "" };
			uint8_t current = 0;
			for (int i = 0; i < line.size(); i++) {
				char letter = line[i];
				if (letter == ' ') {
					current++;
					continue;
				}
				else if (letter == '\n') break;
				arr[current] += letter;
			}

			currentObj = GameObjectClone(allObjects[std::stoi(arr[0])]);
			currentObj.position = Vector2<int>(std::stoi(arr[1]), std::stoi(arr[2]));
			currentObj.scale = Vector2<float>(std::stof(arr[3]), std::stof(arr[4]));
			sceneObjs.push_back(currentObj);
		}
	}
	else std::cout << ("Scenes/" + (std::string)name + std::string(".txt"));
}

Scene::~Scene() {
	Save();
}

void Scene::Save() {
	std::ofstream data((std::string)"Scenes/" + this->name + (std::string)".txt");
	for (auto it = sceneObjs.begin(); it != sceneObjs.end(); it++) {
		data << it->originalGameObject->id << " "
			<< it->position.x << " " << it->position.y << " "
			<< it->scale.x << " " << it->scale.y << "\n";

	}
	data.close();
}

void Scene::Draw() {

};

void Scene::Update() {

}

void Scene::Gui() {

}