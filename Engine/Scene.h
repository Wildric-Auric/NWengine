
#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include<list>

class Scene {
public:
	std::string name;
	std::list<GameObject> sceneObjs;
	std::list<Sprite*> drawList;
	
	//Adds new empty GameObject to the scene objs container
	GameObject& AddObject();

	void DeleteObject(uint32 index);
	void DeleteObject(std::string name);

	//Get gameobject in scene by name
	GameObject* GetGameObject(std::string name);
	GameObject* GetGameObject(const uint32& position);
	//Get last object added to the scene
	GameObject* GetGameObject(); 
	//Deprecated, we keep things sorted at each insertion now
	void SortScene();
	//Add sprite to draw list
	void Render(Sprite* sprite);
	//Iterate over drawlist and delete sprites that aren't rendered, otherwise the action in taken only at next scene Draw
	void ForceRenderStop();
	//Refind a position where to insert sprite in the draw list, called internally when layer changes
	void Rearrange(Sprite* sprite);
	//Rename, mostly used internally and if user wants to change obj name
	const std::string& Rename(const std::string& newName, GameObject* obj);

	Scene(std::string name);
	~Scene();
	void LoadScene();
	void Start();
	void Update();
	void SetUp(); //Sets up needed static variables and other things
	void Draw();
	void Save();

	//This static method calls Update on current scene if not null
	static void UpdateActiveScene();

	static Scene* currentScene;
};