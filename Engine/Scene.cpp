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
		
		while (j>=0 && layer < layer0) {
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


	for (auto pair : ptr->components) {
		delete (GameComponent*)(pair.second);
	}

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

	SortScene();
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

void Scene::Save() {
	std::fstream data((std::string)"Scenes\\" + std::string(this->name) + ".NWscene",
		std::ios::binary | std::ios::out | std::ios::trunc);
	for (std::list<GameObject>::iterator iter = sceneObjs.begin(); iter != sceneObjs.end(); iter++) {
		iter->Serialize(&data, 0);
	}
	int temp = 0;
	data.write((const char*)&temp, 4);
	data.close();
}

void Scene::LoadScene() {

	currentScene = this;
	sceneObjs.clear(); 
	std::fstream data(std::string("Scenes\\") + this->name + ".NWscene", std::ios::binary | std::ios::in);

	data.seekg(0, std::ios::end);
	if (data.tellg() < 1) {
		data.close();
		return;
	}
	data.seekg(0, std::ios::beg);

	int sizeBuffer = 0;
		
	data.read((char*)&sizeBuffer, sizeof(int));
	if (sizeBuffer < 1) {
		delete[] name;
		data.close();
		return;
	}
	char* name = new char[sizeBuffer+1];
	data.read(name, sizeBuffer);
	name[sizeBuffer] = '\0';

	this->AddObject(GameObject());
	sceneObjs.back().Rename(name);

	int flag = Scene::currentScene->sceneObjs.back().Deserialize(&data, 0);
	while (flag == 1) {
		flag = Scene::currentScene->sceneObjs.back().Deserialize(&data, 0);
		if (flag == 0) break;
	}

	delete[] name;
	data.close();
};




static uint16 ind = 0; //Indentation

Scene::~Scene() {
	Save();
	for (std::list<GameObject>::iterator it = sceneObjs.begin(); it != sceneObjs.end(); it++) {
		for (auto pair : it->components) 
			delete (GameComponent*)pair.second;
	}
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