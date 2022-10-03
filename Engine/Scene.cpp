#include "Scene.h"
#include<string>
#include<fstream>
#include"GameObject.h"
#include"RessourcesLoader.h"
#include <vector>
#include <iostream>
#include "ScriptManager.h"
#include <Parser.h>
#include <Components.h>
#include <NWstd.h>
#include "Animator.h"

static std::string subKeys[] = {
	"SortingLayer", "Texture", "Shader",  "Position", "Scale",
	"Position", "Rotation", "Transform", "Offset", "Name"

};

Scene::Scene(const char* name) {
	this->name = _strdup(name);
};

void Scene::SortScene() {
	//DEPRECATED:: The ordering layer is used to calcutlate value for depth buffer, 
	//				fragment are saved from draw, no need to sort, which is useful for future batch implementation (soon), however alpha blending cannot work 
	//Insertion sort ; It won't be called everyframe 
	auto it = drawList.begin();
	for (uint16 i = 1; i < drawList.size(); i++) {
		std::advance(it, 1);
		GameObject* temp = *it;
		int j = i - 1;
		int16 layer0 = -2000;
		int16 layer  =  -2000;
		Sprite* sprite0 = temp->GetComponent<Sprite>();
		auto it0 = drawList.begin();
		std::advance(it0, j);
		Sprite* sprite  = (*it0)->GetComponent<Sprite>();
		if (sprite0 != nullptr) layer0 = sprite0->sortingLayer;
		
		if (sprite != nullptr) layer = sprite->sortingLayer;
		
		while (j>=0 && layer > layer0) {
				GameObject* j0 = *it0;
				std::advance(it0, 1);
				auto j1 = it0;
				*j1 = j0;
				j--;
				if (j == -1) continue;
				std::advance(it0, -2);
		};
		std::advance(it0, 1);
		if (j == -1) std::advance(it0,-2);
		*it0 = temp;
	}
}

void Scene::AddObject(GameObject goc) {
	sceneObjs.push_back(goc);
	sceneObjs.back().Rename("new GameObject");
	drawList.push_back(&sceneObjs.back());
}

void Scene::DeleteObject(uint32 index) {
	auto it1 = sceneObjs.begin();
	std::advance(it1, index);
	GameObject* ptr = &(*it1);
	uint32 count = 0;

	for (auto pair : ptr->components ) {
		delete pair.second;
	} //Maybe should add delete gameobject by type name
	ptr->components.clear();

	for (auto it = drawList.begin(); it != drawList.end(); it++) {
		GameObject* ptr1 = *it;
		if (ptr1 == ptr) {
			auto it = drawList.begin();
			std::advance(it, count);
			drawList.erase(it);
			break;
		}
		count++;
	}
	sceneObjs.erase(it1);
};

void Scene::DeleteObject(std::string name) {
	return;
}

GameObject* Scene::GetGameObject(std::string name) {
	for (auto it = sceneObjs.begin(); it != sceneObjs.end(); ++it) {
		if (it->name == name) return &(*it);
	}
	return nullptr;
}

void Scene::Draw() {

	//SortScene();
	auto it = drawList.begin();
	while (it != drawList.end()) {
		if (!(*it)->isRendered) {
			it = drawList.erase(it);
			continue;
		}
		(*it)->Draw(0);
		it++;
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

	parser.Parse("Scenes\\" + (std::string)name + std::string(".NWscene"));

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
				uint32 temp = std::stoi(value);
				sprite->SetSortingLayer(temp);
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
				script->script = ScriptManager::CreateScript(value, go);
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
};





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
			//add("Script:", 1);

				add("Name:", 1);
				//std::cout <<  << std::endl;
				add(script->script->GetName(), 0, 1, 1);
				add("end", -1);

			//add("end", -1);

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
	for (GameObject obj : sceneObjs) {
		Script* scr = obj.GetComponent<Script>();
		Animator* animator = obj.GetComponent<Animator>();
		ParticleSystem* ps = obj.GetComponent<ParticleSystem>();

		if (scr == nullptr) goto n0;
		if (scr->script == nullptr) goto n0;
		#ifdef NW_GAME_BUILD
		scr->script->Update();
		#endif
	n0:
		if (animator == nullptr) goto n1;
		animator->Update();

	n1:
		if (ps != nullptr) ps->Update();
		continue;
	}
};

bool Scene::GuiActive = false;
Scene* Scene::currentScene = nullptr;


void Scene::Gui() {

}