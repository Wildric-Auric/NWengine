#include "Scene.h"
#include<string>
#include<fstream>
#include"GameObject.h"
#include"RessourcesLoader.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include <iostream>
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

							else if (state[state.size() - 2] == "Collider") {
								if (state[state.size() - 1] == "Position")
									continue;//MODIFY POSITION

								else if (state[state.size() - 1] == "Scale")
									continue;
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
			<< "OriginalGameObject:{" << it->originalGameObject->name << "}\n" //TODO::Fix this by changing objects id to its names
			<< "Position:{"
			<< it->position.x << "," << it->position.y << "}\n"
			<< "Scale:{"
			<< it->scale.x << "," << it->scale.y << "}\n";
		if (it->GetComponent<Collider>() != nullptr) {
			data << "Collider:\n"
				 << "end\n";
		};
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
	if (Scene::GuiActive) {
		ImGui::Begin("Scene editor", 0, ImGuiWindowFlags_MenuBar);
		if (currentScene != nullptr) {
			unsigned int num = 0;
			std::string c = "slider scale?";
			std::string d = "slider position?";


			ImGui::Checkbox(c.c_str(), &slider0);
			ImGui::Checkbox(d.c_str(), &slider1);
			for (auto it = currentScene->sceneObjs.begin(); it != currentScene->sceneObjs.end(); it++) {
				num += 1;
				//TODO::Add names to GameObjectClone
				std::string label = it->name + std::to_string(num);
				ImGui::LabelText(label.c_str(),"");
				std::string a = "scale" + std::to_string(num);
				std::string b = "position" + std::to_string(num);

				


				if (slider0) 
					ImGui::SliderFloat2(a.c_str(), &(it->scale.x), -5.0f, 5.0f, "%.3f", 1.0f);
				else
					ImGui::InputFloat2(a.c_str(), &(it->scale.x));
				if (slider1)
					ImGui::SliderInt2(b.c_str(), &(it->position.x), -1000, +1000);
				else 
					ImGui::InputInt2(b.c_str(), &(it->position.x));

				//ImGui::SliderFloat2()
			}
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close", "Ctrl+W")) Scene::GuiActive = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		/*ImGui::EndChild*/
		ImGui::End();
	}

}