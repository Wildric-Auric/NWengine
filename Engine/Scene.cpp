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
#include <Parser.h>
#include <Components.h>
#include <NWstd.h>
static std::string subKeys[] = {
	"SortingLayer", "Texture", "Shader",  "Position", "Scale",
	"Position", "Rotation", "Transform", "Offset", "Name"

};

Scene::Scene(const char* name) {
	this->name = _strdup(name);
};

void Scene::SortScene() {
	//Insertion sort ; It won't be called everyframe 
	for (uint16 i = 1; i < drawList.size(); i++) {
		GameObject* temp = drawList[i];
		int j = i - 1;
		int16 layer0 = -2000;
		int16 layer  =  -2000;
		Sprite* sprite0 = temp->GetComponent<Sprite>();
		Sprite* sprite  = drawList[j]->GetComponent<Sprite>();
		if (sprite0 != nullptr) layer0 = sprite0->sortingLayer;
		
		if (sprite != nullptr) layer = sprite->sortingLayer;
		
		while (j>=0 && layer > layer0) {
				GameObject* temp = drawList[i];
				drawList[j+1] = drawList[j];
				j--;
		}
		drawList[j + 1] = temp;
	}
}

void Scene::AddObject(GameObject goc) {
	sceneObjs.push_back(goc);
	drawList.clear(); /*	Clearing drawList and filling it again is done because of the contiguous
							nature of vectors which makes reallocation change initial adresses
							TODO::Optimize this by using deque instead of Vector
					  */
	for (uint16 i = 0; i < sceneObjs.size(); ++i) {
		drawList.push_back(&sceneObjs[i]);
	} 
}

void Scene::Draw() {

	SortScene();

	for (auto it = drawList.begin(); it != drawList.end(); ++it) {
		(*it)->Draw(0);
	}
}
void Scene::LoadScene() {

	currentScene = this;
	sceneObjs.clear(); 
	Parser parser = Parser();
	uint8 index = 0;

	GameObject* go         =    nullptr;
	Sprite* sprite         =    nullptr;
	Transform* transform   =    nullptr;
	Collider2* collider    =    nullptr;
	Script* script         =    nullptr;
	Camera* cam            =	nullptr;


	parser.Parse("Scenes/" + (std::string)name + std::string(".NWscene"));

	std::vector<std::string> stack;
	for (std::string value : parser.rawData) {
		//Poping from stack -------------------------------------------------------------------

		if (value == "end") {
			stack.pop_back();
			index = 0;
			continue;
		}

		//Adding keys to stack -----------------------------------------------------------------
	    if (value == "GameObject") {
			this->AddObject(GameObject());
			go = &sceneObjs.back();
			stack.push_back(value);
			continue;
		}

	    if (value == "Sprite") {
			sprite = sceneObjs.back().AddComponent<Sprite>();
			stack.push_back(value);
			continue;
		}

		if (value == "Transform") {
			transform = sceneObjs.back().AddComponent<Transform>();
			stack.push_back(value);
			continue;
		}

		if (value == "Collider") {
			collider = sceneObjs.back().AddComponent<Collider2>();
			stack.push_back(value);
			continue;
		}

		if (value == "Script") {
			script = sceneObjs.back().AddComponent<Script>();
			stack.push_back(value);
			continue;
		}

		if (value == "Camera") {
			cam = sceneObjs.back().AddComponent<Camera>();
			Camera::ActiveCamera = cam;
			stack.push_back(value);
			continue;
		}
		bool br = 0;
		for (std::string temp : subKeys) {
			if (value == temp) {
				stack.push_back(value);
				br = 1;
				break;
			}
		}
		if (br) continue;
		//Changing values of components ------------------------------------------------------------------
		if (stack[stack.size() - 2] == "GameObject" && stack.back() == "Name") {
			go->name = value;
			continue;
		}
 
		if (stack[stack.size() - 2] == "Transform") {

			if (stack.back() == "Position") {
				*(&transform->position.x + index) = std::stoi(value);
				index += 1;
				continue;
			}

			if (stack.back() == "Scale") {
				*(&transform->scale.x + index) = std::stof(value);
				index += 1;
				continue;
			}
		}

		if (stack[stack.size() - 2] == "Sprite") {

			if (stack.back() == "SortingLayer") {
				sprite->sortingLayer = std::stoi(value);
				continue;
			}

			if (stack.back() == "Texture") {
				static bool booAlpha = 0;
				static std::string texPath = "";

				if (index == 0) {
					texPath = value;
					index += 1;
					continue;
				};

				if (index == 1) {
					booAlpha = std::stoi(value);
					index += 1;
					continue;
				}
				if (index == 2) {
					sprite->SetTexture(texPath, booAlpha, std::stoi(value));
					index += 1;
					continue;
				}
			}

			if (stack.back() == "Shader") {
				sprite->SetShader(value);
				continue;
			}
		}

		if (stack[stack.size() - 2] == "Collider") {

			if (stack.back() == "Offset") {
				*(&collider->offset.x + index) = std::stoi(value);
				index += 1;
				continue;
			}
		}

		if (stack[stack.size() - 2] == "Script") {

			if (stack.back() == "Name") {
				script->script = CreateScript(value, go);
				continue;
			}
		}

		if (stack[stack.size() - 2] == "Camera") {
			if (stack.back() == "Position") {
				*((&cam->position.x) + index) = std::stoi(value);
				index += 1;
				continue;
			}

			if (stack.back() == "Rotation") {
				cam->rotation = std::stoi(value);
				continue;
			}
		}

	}
}

static std::ofstream data;
static uint16 ind = 0; //Indentation

Scene::~Scene() {
	Save();
}


static void add(std::string str, int8 indIncrement = 0,bool endline = 1, bool isString = 0) {
	data << stringMul("\t", ind) << stringMul("\"", isString) <<str <<stringMul("\"", isString) <<stringMul("\n", endline);
	ind += indIncrement;
};

void Scene::Save() {
	data.open((std::string)"Scenes/" + std::string(this->name) + (std::string)".NWscene");
	for (auto go = sceneObjs.begin(); go != sceneObjs.end(); go++)
	{
		Sprite* sprite			= go->GetComponent<Sprite>();
		Transform* transform    = go->GetComponent<Transform>();
		Collider2* collider	    = go->GetComponent<Collider2>();
		Script* script          = go->GetComponent<Script>();
		Camera* cam             = go->GetComponent<Camera>();
		
		add("GameObject:", 1);
		add("Name:", 1);
		add(go->name, 0, 1, 1);
		add("end", -1);
		if (transform != nullptr) {
			add("Transform:", 1);
				add("Position:",  1);
					add(std::to_string(transform->position.x));
					add(std::to_string(transform->position.y));
				add("end", -1);

				add("Scale:", 1);
					add(std::to_string(transform->scale.x));
					add(std::to_string(transform->scale.y));
				add("end", -1);
			add("end", -1);
		};

		if (sprite != nullptr) {
			add("Sprite:", 1);

				add("SortingLayer:", 1);
				add(std::to_string(sprite->sortingLayer));
				add("end", -1);

				add("Texture:", 1);
					add(sprite->texture->name, 0, 1, 1);
					add(std::to_string(sprite->texture->alpha));
					add(std::to_string(sprite->texture->repeat));
				add("end", -1);

				add("Shader:", 1);
					add(sprite->shader->name, 0, 1, 1);
				add("end", -1);

			add("end", -1);

		};

		if (collider != nullptr) {
			//TODO::Complete collider save
			add("Collider:");

			add("end");
		};

		if (script != nullptr) {
			add("Script:", 1);
				add("Name:", 1);
				add(script->script->name(), 0, 1, 1);
				add("end", -1);

			add("end", -1);

		};

		if (cam != nullptr) {
			add("Camera:", 1);

				add("Position:", 1);
				add(std::to_string(cam->position.x));
				add(std::to_string(cam->position.y));
				add("end", -1);

				add("Rotation:", 1);
				add(std::to_string(cam->rotation));
				add("end", -1);


			add("end", -1);
		};

		add("end", -1);
	}
	data.close();
}


void Scene::Update() {

};

bool Scene::GuiActive = false;
Scene* Scene::currentScene = nullptr;


void Scene::Gui() {

}