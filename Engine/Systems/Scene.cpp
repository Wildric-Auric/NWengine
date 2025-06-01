#include "Scene.h"
#include "Batch.h"
#include "Camera.h"
#include "GameObject.h"
#include <string>
#include <vector>

#define AllocSceneObj()		  new SceneObjNode
#define DeallocSceneObj(node) delete node

#define AllocSceneActiveObj()		new SceneActiveObjNode
#define DeallocSceneActiveObj(node) delete node

Scene::Scene(const std::string& name) { this->name = name; };

void Scene::SortScene() {} // Deprecated

void Scene::Rearrange(Sprite* sprite) {
	// Adds the object if not in the list
	for(auto iter = drawList.begin(); iter != drawList.end(); ++iter) {
		if(*iter != sprite)
			continue;
		drawList.erase(iter);
		break;
	}
	Render(sprite);
}

void Scene::Render(Sprite* sprite) {
	// TODO::Make drawList a list of sprities not gameobjects
	std::list<Sprite*>::iterator iter  = drawList.begin();
	int64						 layer = sprite->sortingLayer;
	// Insert at beginning
	if(drawList.size() < 1 || layer <= (*iter)->sortingLayer) {
		drawList.insert(drawList.begin(), sprite);
		return;
	}
	if(drawList.size() == 1) {
		drawList.insert(drawList.end(), sprite);
		return;
	}
	// At the middle
	while(iter != --drawList.end()) {
		auto  a	 = iter;
		auto  b	 = ++iter;
		int64 lA = (*a)->sortingLayer;
		int64 lB = (*b)->sortingLayer;
		if(lA <= layer && lB >= layer) {
			drawList.insert(b, sprite);
			return;
		}
	}
	// At the end
	drawList.insert(drawList.end(), sprite);
}

void Scene::ForceRenderStop() {
	auto it = this->drawList.begin();
	while(it != this->drawList.end()) {
		if(((*it) == nullptr) || !(*it)->_isRendered) {
			it = this->drawList.erase(it);
			continue;
		}
		it++;
	}
}

GameObject* Scene::GetGameObject(const uint32& position) {
	if(position >= _sceneObjs.size)
		return 0;
	uint32		  i = 0;
	SceneObjNode* n = _sceneObjs.first;
	while(i++ != position) {
		n = n->next;
	}
	return &n->cont;
}

GameObject* Scene::GetGameObject() {
	if(_sceneObjs.size == 0)
		return 0;
	return &_sceneObjs.last->cont;
}

GameObject& Scene::DirectAddObject() {
	SceneObjNode* obj  = AllocSceneObj();
	SceneObjNode* last = _sceneObjs.last;
	if(_sceneObjs.first == 0)
		_sceneObjs.first = obj;
	if(last != 0)
		last->next = obj;
	_sceneObjs.last = obj;
	obj->prev		= last;
	++_sceneObjs.size;
	return obj->cont;
}

GameObject& Scene::AddObject() { return AddObject("new GameObject"); }

GameObject& Scene::AddObject(const char* n) {
	GameObject& obj = DirectAddObject();
	Rename(n, &obj);
	EnableObject(_sceneObjs.last);
	return obj;
}

SceneObjNode* Scene::DeleteObject(SceneObjNode* obj) {
	GameObject*	  ptr = &obj->cont;
	SceneObjNode* ret = obj->next;
	if(_sceneObjs.last == obj)
		_sceneObjs.last = obj->prev;
	if(_sceneObjs.first == obj)
		_sceneObjs.first = obj->next;
	if(obj->prev)
		obj->prev->next = obj->next;
	if(obj->next)
		obj->next->prev = obj->prev;
	ptr->DeleteComponents();
	if(_autoCache)
		DeleteFromCache(*ptr);
	DeallocSceneObj(obj);
	--_sceneObjs.size;
	return ret;
}

SceneActiveObjNode* Scene::DisableObject(SceneActiveObjNode* obj) {
	SceneActiveObjNode* ret = obj->next;
	if(_activeObjs.last == obj)
		_activeObjs.last = obj->prev;
	if(_activeObjs.first == obj)
		_activeObjs.first = obj->next;
	if(obj->prev)
		obj->prev->next = obj->next;
	if(obj->next)
		obj->next->prev = obj->prev;
	obj->cont->isActive = false;
	DeallocSceneActiveObj(obj);
	--_activeObjs.size;
	return ret;
}

SceneActiveObjNode* Scene::EnableObject(SceneObjNode* content) {
	SceneActiveObjNode* obj	 = AllocSceneActiveObj();
	SceneActiveObjNode* last = _activeObjs.last;
	if(content->isActive)
		return 0;
	if(_activeObjs.first == 0)
		_activeObjs.first = obj;
	if(last != 0)
		last->next = obj;
	_activeObjs.last = obj;
	obj->prev		 = last;
	++_activeObjs.size;
	obj->cont		  = content;
	content->isActive = 1;
	return obj;
}

void Scene::DeleteLastObject() { DeleteObject(_sceneObjs.last); }
void Scene::DeleteFirstObject() { DeleteObject(_sceneObjs.first); }
void Scene::DeleteObject(GameObject* obj) {
	for(SceneObjNode* node = _sceneObjs.first; node; node = node->next) {
		if(&node->cont != obj)
			continue;
		DeleteObject(node);
		return;
	}
}

void Scene::DeleteObject(uint32 index) {
	uint32		  i = 0;
	SceneObjNode* n = _sceneObjs.first;
	while(i++ != index) {
		n = n->next;
	}
	DeleteObject(n);
};

void Scene::DeleteObject(const std::string& name) {
	SceneObjNode* n = _sceneObjs.first;
	while(n) {
		if(n->cont.name == name) {
			DeleteObject(n);
			return;
		}
		n = n->next;
	}
}

void Scene::DeleteCurrentObj() { _shouldDelObj = true; }

GameObject* Scene::GetGameObject(std::string name) {
	SceneObjNode* n = _sceneObjs.first;
	while(n) {
		if(n->cont.name == name)
			return &n->cont;
		n = n->next;
	}
	return 0;
}

void Scene::Draw() {
	std::list<Sprite*>::iterator						   it = drawList.end();
	std::unordered_map<int, std::vector<Batch*>>::iterator it0;

	int64 lastLayer = 0x7FFFFFFFFFFFFFFF;
	int64 temp		= 0;

	while(it != drawList.begin()) {
		--it;
		if(!(*it)->_isRendered) {
			it = drawList.erase(it);
			continue;
		}
		// Drawing batches
		temp = (*it)->sortingLayer;
		if((temp != lastLayer) && ((it0 = Batch::batchMap.find(lastLayer)) != Batch::batchMap.end())) {
			for(Batch* batch : it0->second)
				batch->Draw();
		}
		(*it)->attachedObject->Draw();
		lastLayer = temp;
	}
	// Drawing last layer batches
	if((it0 = Batch::batchMap.find(temp)) != Batch::batchMap.end()) {
		for(Batch* batch : it0->second)
			batch->Draw();
	}
}

void Scene::DeferredDisableCurrentGameObject() { _shouldDisable = 1; }

void Scene::MakeCurrent() { currentScene = this; }

bool Scene::IsCurrent() { return Scene::currentScene == this; }

Scene::~Scene() {
	// Delete all components
	SceneActiveObjNode* n0 = _activeObjs.first;
	while(n0) {
		DeleteObject(n0->cont);
		n0 = DisableObject(n0);
	}
	SceneObjNode* n = _sceneObjs.first;
	while(n) {
		n = DeleteObject(n);
	}
}

void Scene::Start() {
	for(SceneActiveObjNode* n = _activeObjs.first; n; n = n->next) {
		GameObject* obj = &n->cont->cont;
		for(auto iter = obj->components.begin(); iter != obj->components.end(); iter++) {
			iter->second->Start();
		}
		if(_autoCache)
			AddToCache(*obj, n->cont);
	}
}

void Scene::Update() {
	for(SceneActiveObjNode* n = _activeObjs.first; n; n = n->next) {
		GameObject* obj = &n->cont->cont;
		for(auto iter = obj->components.begin(); iter != obj->components.end(); iter++) {
			iter->second->Update();
		}
		if(_shouldDelObj) {
			_shouldDelObj = 0;
			DeleteObject(n->cont);
			n = DisableObject(n);
			n = n->prev;
		}
		if(_shouldDisable) {
			_shouldDisable = 0;
			DisableObject(n);
		}
	}
};

const std::string& Scene::Rename(const std::string& newName, GameObject* obj) {
	uint32 n  = 0;
	obj->name = newName;
	while(1) {
		bool br = 1;
		for(SceneObjNode* node = _sceneObjs.first; node; node = node->next) {
			if(node->cont.name == obj->name && obj != &node->cont) {
				n += 1;
				br		  = 0;
				obj->name = (newName + std::to_string(n));
			}
		}
		if(br)
			break;
	}
	if(n == 0)
		obj->name = newName;
	else
		obj->name = newName + std::to_string(n);
	return obj->name;
}

Scene*			 Scene::currentScene = nullptr;
std::list<Scene> Scene::_scenes;

void Scene::SetUp() {
	for(SceneActiveObjNode* node = _activeObjs.first; node; node = node->next) {
		auto iter = node->cont->cont.components.find(CameraID);
		if(iter == node->cont->cont.components.end())
			continue;
		((Camera*)iter->second)->Use();
		break;
	}
}

void Scene::SetPath(const std::string& path) { name = path; }

Scene& Scene::CreateNew(const std::string& path) {
	_scenes.push_back(Scene(path));
	return _scenes.back();
}

Scene* Scene::GetScene(const std::string& path) {
	for(auto iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		if(iter->name == path) {
			return &*iter;
		}
	}
	return nullptr;
}

bool Scene::DeleteScene(const std::string& path) {
	for(auto iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		if(iter->name == path) {
			_scenes.erase(iter);
			return 1;
		}
	}
	return 0;
}

void Scene::Destroy() { _scenes.clear(); }

Scene* Scene::GetCurrent() { return currentScene; }

void Scene::UpdateActiveScene() {
	if(Scene::currentScene != nullptr)
		Scene::currentScene->Update();
}

void Scene::DeferredDeleteCurrentGameObject() { _shouldDelObj = 1; }

void Scene::FillCache() {
	if(cache.size() <= 0)
		return;
	for(SceneObjNode* node = _sceneObjs.first; node; node = node->next) {
		AddToCache(node->cont, node);
	}
}

void Scene::FillCache(cacheCondProc proc) {
	AddToCache(proc);
	for(SceneObjNode* node = _sceneObjs.first; node; node = node->next) {
		AddToCache(node->cont);
	}
}

void Scene::DestroyCache(cacheCondProc proc) { cache.erase(proc); }

void Scene::DestroyCache() { cache.clear(); }

void Scene::AddToCache(GameObject& obj, SceneObjNode* node) {
	for(auto& key : cache) {
		if(key.first(&obj))
			key.second[&obj] = node;
	}
}

void Scene::AddToCache(cacheCondProc key, GameObject& obj) {
	AddToCache(key);
	if(key(&obj))
		cache[key][&obj] = _sceneObjs.last;
}

void Scene::DeleteFromCache(cacheCondProc key, GameObject& obj) {
	auto iter = cache.find(key);
	if(iter == cache.end())
		return;
	iter->second.erase(&obj);
}

void Scene::AddToCache(GameObject& obj) { AddToCache(obj, _sceneObjs.last); }

void Scene::AddToCache(cacheCondProc proc) {
	auto iter = cache.find(proc);
	if(iter == cache.end())
		cache[proc] = {};
}

void Scene::DeleteFromCache(GameObject& obj) {
	for(auto& key : cache) {
		key.second.erase(&obj);
	}
}

int Scene::CacheMap(cacheCondProc key, mapProc proc, void* data) {
	auto iter = cache.find(key);
	if(iter == cache.end())
		return 0;
	int res = 1;
	int val = 0;
	for(auto pair : iter->second) {
		res = res + proc(pair.first, data);
	}
	return res;
}

GameObject* Scene::GetFirstObjectWith(bool (*f)(GameObject*, void*), void* data) {
	for(SceneObjNode* node = _sceneObjs.first; node; node = node->next) {
		if(f(&node->cont, data))
			return &node->cont;
	}
	return 0;
}

GameObject* Scene::GetFirstObjectWithComponent(uint32 compID) {
	return GetFirstObjectWith([](GameObject* obj, void* compID) -> bool { return obj->GetComponent(*(uint32*)compID); }, &compID);
}

int Scene::ObjMap(mapProc proc, void* data) {
	int res = 1;
	int val = 0;
	for(SceneObjNode* node = _sceneObjs.first; node; node = node->next) {
		res += proc(&node->cont, data);
	}
	return res;
}

void Scene::SetAutoCache(bool val) { _autoCache = val; }

bool Scene::GetAutoCache() { return _autoCache; }
