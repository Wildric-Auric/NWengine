#include "Scene.h"
#include "GameObject.h"
#include "ScriptManager.h"
#include "Components.h"
#include "Camera.h"
#include "Batch.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

Scene::Scene(const std::string& name) {
	this->name = name;
};


void Scene::SortScene() {} //Deprecated

void Scene::Rearrange(Sprite* sprite) {
	//Adds the object if not in the list
	for (auto iter = drawList.begin(); iter != drawList.end(); ++iter)  {
		if (*iter != sprite)
			continue;
		drawList.erase(iter);
		break;
	}
	Render(sprite);
}

void Scene::Render(Sprite* sprite) {
	//TODO::Make drawList a list of sprities not gameobjects
	std::list<Sprite*>::iterator iter = drawList.begin();
	int layer = sprite->sortingLayer;
	//Insert at beginning
	if (drawList.size() < 1 || layer <= (*iter)->sortingLayer) {
		drawList.insert(drawList.begin(), sprite);
		return;
	}
	if (drawList.size() == 1) {
		drawList.insert(drawList.end(), sprite);
		return;
	}
	//At the middle
	while (iter != --drawList.end()) {
		auto a	   = iter;
		auto b	   = ++iter;
		int lA = (*a)->sortingLayer;
		int lB=  (*b)->sortingLayer;
		if (lA <= layer && lB >= layer) {
			drawList.insert(b, sprite);
			return;
		}
	}
	//At the end
	drawList.insert(drawList.end(), sprite);
}

void Scene::ForceRenderStop() {
	auto it = this->drawList.begin();
	while (it != this->drawList.end()) {
		if (((*it) == nullptr) || !(*it)->_isRendered) {
			it = this->drawList.erase(it);
			continue;
		}
		it++;
	}
}

GameObject* Scene::GetGameObject(const uint32& position) {
	if  (position >=  this->sceneObjs.size())
		return nullptr;
	std::list<GameObject>::iterator it = sceneObjs.begin();
	std::advance(it, position);
	return &*it;
}

GameObject* Scene::GetGameObject() {
	if (this->sceneObjs.size() <= 0) return nullptr;
	return &this->sceneObjs.back();
}

GameObject& Scene::AddObject() {
	sceneObjs.push_back(GameObject());
    Rename("new GameObject", &sceneObjs.back());
	return sceneObjs.back();
}

std::list<GameObject>::iterator Scene::DeleteObject(std::list<GameObject>::iterator it) {
	GameObject* ptr = &(*it);
	for (auto pair : ptr->components) {
		delete (GameComponent*)(pair.second);
	}
    if (_autoCache)
        DeleteFromCache(*ptr);
	return sceneObjs.erase(it);
}

void Scene::DeleteObject(uint32 index) {
	auto it1 = sceneObjs.begin();
	std::advance(it1, index);
	DeleteObject(it1);
};

void Scene::DeleteObject(std::string name) {
	auto it1 = sceneObjs.begin();
	while (it1 != sceneObjs.end()) {
		if (it1->name == name) {
			DeleteObject(it1);
			return;
		}
		++it1;
	}
}

void Scene::DeleteCurrentObj() {
	_shouldDelObj = true;
}


GameObject* Scene::GetGameObject(std::string name) {
	for (auto it = sceneObjs.begin(); it != sceneObjs.end(); ++it) {
		if (it->name == name) return &(*it);
	}
	return nullptr;
}

void Scene::Draw() {
	std::list<Sprite*>::iterator it		  = drawList.end();
	std::unordered_map<int, std::vector<Batch*>>::iterator it0;

	int lastLayer							      = 0xFFFFFFFF - 1;
	int temp									  = 0;

	while (it != drawList.begin()) {
		--it;
		if (!(*it)->_isRendered) {
			it = drawList.erase(it);
			continue;
		}
		//Drawing batches
		temp = (*it)->sortingLayer;
		if ( (temp != lastLayer) &&  ( (it0 = Batch::batchMap.find(lastLayer)) != Batch::batchMap.end() )) {
			for (Batch* batch : it0->second)
				batch->Draw();
		}
		(*it)->attachedObj->Draw();
		lastLayer = temp;
	}
	//Drawing last layer batches
	if  ((it0 = Batch::batchMap.find(temp)) != Batch::batchMap.end()) {
		for (Batch* batch : it0->second)
			batch->Draw();
	}
}

void Scene::Save() {
	std::fstream data( this->name, std::ios::binary | std::ios::out | std::ios::trunc);
	for (std::list<GameObject>::iterator iter = sceneObjs.begin(); iter != sceneObjs.end(); iter++) {
		iter->Serialize(&data, 0);
	}
	int temp = 0;
	data.write((const char*)&temp, 4);
	data.close();
}

void Scene::LoadScene() {
	sceneObjs.clear(); 
	std::fstream data(this->name, std::ios::binary | std::ios::in);

	data.seekg(0, std::ios::end);
	if (data.tellg() < 1) {
		data.close();
		return;
	}
	data.seekg(0, std::ios::beg);

	int sizeBuffer = 0;
		
	data.read((char*)&sizeBuffer, sizeof(int));
	if (sizeBuffer < 1) {
		data.close();
		return;
	}
	char* name0 = new char[sizeBuffer+1];
	data.read(name0, sizeBuffer);
	name0[sizeBuffer] = '\0';

	this->AddObject();
	Rename(name0, &sceneObjs.back());
	delete[] name0;

	int flag = Scene::currentScene->sceneObjs.back().Deserialize(&data, 0);
	while (flag == 1) {
		flag = Scene::currentScene->sceneObjs.back().Deserialize(&data, 0);
		if (flag == 0) break;
	}
	data.close();
};

void Scene::MakeCurrent() {
	currentScene = this;
}

bool Scene::IsCurrent() {
	return Scene::currentScene == this;
}


Scene::~Scene() {
	//Delete all components
	for (std::list<GameObject>::iterator it = sceneObjs.begin(); it != sceneObjs.end(); it++) {
		for (auto pair : it->components) 
			delete (GameComponent*)pair.second;
	}
}

void Scene::Start() {	
	for (std::list<GameObject>::iterator obj = sceneObjs.begin(); obj != sceneObjs.end(); obj++) {
		for (std::map<std::string, GameComponent*>::iterator iter = obj->components.begin(); iter != obj->components.end(); iter++) {
			iter->second->Start();
		}
        if (_autoCache)
            AddToCache(*obj, obj);
	}
}

void Scene::Update() {
	std::list<GameObject>::iterator last;

	for (auto obj = sceneObjs.begin(); obj != sceneObjs.end(); obj++) {
		for (auto iter = obj->components.begin(); iter != obj->components.end(); iter++) {
			iter->second->Update();
		}
		if (_shouldDelObj) {
			_shouldDelObj = 0;
			obj = DeleteObject(obj);
			--obj;
		}
	}
	
};

const std::string& Scene::Rename(const std::string& newName, GameObject* obj) {
	uint32 n = 0;
	obj->name = newName;
	while (1) {
		bool br = 1;
		for (auto it = sceneObjs.begin(); it != sceneObjs.end(); it++) {
		    if (it->name == obj->name && &(*it) != obj) {
				n += 1;
				br = 0;
				obj->name = newName + std::to_string(n);
			}
		}
		if (br) break;
	}
	if (n == 0) obj->name = newName;
	else obj->name = newName + std::to_string(n);
	return obj->name;
}

Scene* Scene::currentScene = nullptr;
std::list<Scene> Scene::_scenes;


void Scene::SetUp() {
	for (std::list<GameObject>::iterator obj = sceneObjs.begin(); obj != sceneObjs.end(); obj++) {
		std::map<std::string, GameComponent*>::iterator iter = obj->components.find("Camera");
		if (iter == obj->components.end())
			continue;
		((Camera*)iter->second)->Use();
		break;
	}
}


void Scene::SetPath(const std::string& path) {
	name = path;
}


Scene& Scene::CreateNew(const std::string& path) {
	_scenes.push_back(Scene(path));
	return _scenes.back();
}

Scene* Scene::GetScene(const std::string& path) {
	for (auto iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		if (iter->name == path) {
			return &*iter;
		}
	}
	return nullptr;
}


bool Scene::DeleteScene(const std::string& path) {
	for (auto iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		if (iter->name == path) {
			_scenes.erase(iter);
			return 1;
		}
	}
	return 0;
}

void Scene::Destroy() {
	_scenes.clear();
}

Scene* Scene::GetCurrent() {
	return currentScene;
}


void Scene::UpdateActiveScene() {
	if (Scene::currentScene != nullptr)
		Scene::currentScene->Update();
}


void Scene::DeferredDeleteCurrentGameObject() {
    _shouldDelObj = 1;
}

void Scene::FillCache() {
    if (cache.size() <= 0) return;
    for (auto obj = sceneObjs.begin(); obj != sceneObjs.end(); ++obj)  {
        AddToCache(*obj, obj);
    }
}

void Scene::FillCache(cacheCondProc proc) {
    AddToCache(proc);
    for (auto obj = sceneObjs.begin(); obj != sceneObjs.end(); ++obj) {
        AddToCache(*obj);
    }
}

void Scene::DestroyCache(cacheCondProc proc) {
    cache.erase(proc);
}

void Scene::DestroyCache() {
    cache.clear();
}
    
void Scene::AddToCache(GameObject& obj, std::list<GameObject>::iterator iter) {
    for (auto& key : cache) {
        if (key.first(&obj))
            key.second[&obj] = iter;
    }
}

void Scene::AddToCache(cacheCondProc key, GameObject& obj) {
    AddToCache(key);
    if (key(&obj))
        cache[key][&obj] = sceneObjs.end();
}

void Scene::DeleteFromCache(cacheCondProc key, GameObject& obj) {
    auto iter = cache.find(key);
    if (iter == cache.end()) return;
    iter->second.erase(&obj);
}

void Scene::AddToCache(GameObject& obj) {
    AddToCache(obj, sceneObjs.end()); 
}

void Scene::AddToCache(cacheCondProc proc) {
    auto iter = cache.find(proc);
    if (iter == cache.end()) 
        cache[proc] =  {};
}

void Scene::DeleteFromCache(GameObject& obj) {
    for (auto& key : cache) {
        key.second.erase(&obj);
    }
}

int Scene::CacheMap(cacheCondProc key, mapProc proc, void* data) {
    auto iter = cache.find(key);
    if ( iter == cache.end()) return 0;
    int res = 1;
    int val = 0;
    for (auto pair: iter->second) {
        res = res + proc(pair.first, data);
    }
    return res;
}

int Scene::ObjMap(mapProc proc, void* data) {
    int res = 1;
    int val = 0;
    for (auto o = sceneObjs.begin(); o != sceneObjs.end(); ++o) {
        res += proc(&*o, data);
    }
    return res;
}

void Scene::SetAutoCache(bool val) {
    _autoCache = val;
}

bool Scene::GetAutoCache() {
    return _autoCache;
}
