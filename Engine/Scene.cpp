#include "Scene.h"
#include<string>
#include<fstream>
#include"GameObject.h"
#include"RessourcesLoader.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include <iostream>
#include "ScriptManager.h"


static GameObjectClone currentObj;

Scene::Scene(const char* name) {
	this->name = _strdup(name);
};

void Scene::Draw() {
	for (auto it = sceneObjs.begin(); it != sceneObjs.end(); ++it) {
		(*it).Draw(0);
	}
}
void Scene::LoadScene() {
	sceneObjs.clear(); 
	std::ifstream file("Scenes/" + (std::string)name + std::string(".txt"));



	if (file) {
		std::vector<std::string> state;
		/*
		possible States:
		GameObjectClone
		GameComponent
		*/

		bool hasCollider = false;
		bool hasScript = false;
		for (std::string line; std::getline(file, line);) {
			std::string key = "";
			bool keyPassed = false;
			for (int i = 0; i <= line.size(); i++) {

				char currentChar = line[i];

				//Getting key and setting state
				if (i == line.size()) {
					if (key == "end") {
						state.pop_back();
						continue;
					}
				}
				if (currentChar == ' ' || currentChar == '\t') continue;

				if (currentChar == ':') {
					keyPassed = true;
					state.push_back(key);

					if (key.find('"') != -1 && state[state.size() - 2] == "GameObjectClones") {
						sceneObjs.push_back(GameObjectClone());
						hasCollider = false;
					}

					if (key == "Collider")
						hasCollider = true;

					if (key == "Script")
						hasScript = true;
					key = "";
					continue;

				}

				key += currentChar;
				if (!keyPassed) continue;

				//Dealing with values if any
				if (currentChar == '{') {
					key = "";
					std::string arg = "";
					i += 1;
					//parsing arguments
					int argNum = 0;
					while (1) {
						currentChar = line[i];
						i += 1;
						if (currentChar == ',' || currentChar =='}') {
							if (state[state.size() - 1] == "OriginalGameObject")
								sceneObjs[sceneObjs.size() - 1] = GameObjectClone(&objects[arg], state[state.size() - 2].substr(1, state[state.size() - 2].size()-2).c_str());
							if (hasCollider) sceneObjs[sceneObjs.size() - 1].AddComponent<Collider>();
							if (hasScript) sceneObjs[sceneObjs.size() - 1].AddComponent<Script>();

							if (state[state.size() - 2].find('"') != -1) {

								if (state[state.size() - 1] == "Position") {
									*sceneObjs[sceneObjs.size() - 1].position[argNum] = std::stoi(arg);
									argNum += 1;
								}

								else if (state[state.size() - 1] == "Scale") {
										*sceneObjs[sceneObjs.size() - 1].scale[argNum] = std::stof(arg);
										argNum += 1;
								}

							}
							//Game Components settings
							else if (state[state.size() - 2] == "Collider") {
								if (state[state.size() - 1] == "Position")
									continue;//MODIFY POSITION

								else if (state[state.size() - 1] == "Scale")
									continue;
							}

						     if (state[state.size() - 1] == "Script") {
								sceneObjs[sceneObjs.size() - 1].GetComponent<Script>()->script = CreateScript(arg,&sceneObjs[sceneObjs.size() - 1]);
							 }
							arg = "";
							if (currentChar == '}') break;
							continue;
						}
						arg += currentChar;
					}
					state.pop_back();
					if (i >= line.size()) break;
				}
			}
		}
	}



	currentScene = this;
}

Scene::~Scene() {
	Save();
}

void Scene::Save() {
	std::ofstream data((std::string)"Scenes/" + this->name + (std::string)".txt");
	data << "GameObjectClones:\n";
	for (auto it = sceneObjs.begin(); it != sceneObjs.end(); it++) {
		data << '"' << it->name << '"' << ":\n"
			<< "OriginalGameObject:{" << it->originalGameObject->name << "}\n"
			<< "  Position:{"
			<< it->position.x << "," << it->position.y << "}\n"
			<< "  Scale:{"
			<< it->scale.x << "," << it->scale.y << "}\n";
		if (it->GetComponent<Collider>() != nullptr) {
			data << "  Collider:\n"
				 << "  end\n";
		};
		if (it->GetComponent<Script>() != nullptr) {
			std::string scriptName = typeid(*it->GetComponent<Script>()->script).name(); //DevNote: Typeid is compiler dependent so pay attention
			data << "  Script:{" << scriptName.substr(6, scriptName.length() - 1) <<"}\n";

		}
		data <<"end\n";

	}
	data << "end\n";
	data.close();
}


void Scene::Update() {

};

bool Scene::GuiActive = false;
Scene* Scene::currentScene = nullptr;

static bool slider0 = true;
static bool slider1 = true;

void Scene::Gui() {

}