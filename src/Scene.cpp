#include "Scene.h"
#include<string>
#include<fstream>
#include"GameObject.h"
#include"RessourcesLoader.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
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
		for (std::string line; std::getline(file, line);)
		{
			if (line[0] == ' ' || !line[0] || line[0] == '\n') break;
			std::string arr[6] = { "" };
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

			currentObj = GameObjectClone(allObjects[std::stoi(arr[0])], arr[5].c_str());
			currentObj.position = Vector2<int>(std::stoi(arr[1]), std::stoi(arr[2]));
			currentObj.scale = Vector2<float>(std::stof(arr[3]), std::stof(arr[4]));
			sceneObjs.push_back(currentObj);
		}
	}
	else std::cout << ("Scenes/" + (std::string)name + std::string(".txt"));
	currentScene = this;
}

Scene::~Scene() {
	Save();
}

void Scene::Save() {
	std::ofstream data((std::string)"Scenes/" + this->name + (std::string)".txt");
	for (auto it = sceneObjs.begin(); it != sceneObjs.end(); it++) {
		data << it->originalGameObject->id << " "
			<< it->position.x << " " << it->position.y << " "
			<< it->scale.x << " " << it->scale.y << " "
			<< it->name
			<<"\n";

	}
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