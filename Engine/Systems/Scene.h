
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

	Scene(const std::string& path);

	~Scene();
	//Loads scene from file with "name" as path
	void LoadScene();
	//Set the scene as current.Will be updated and drawn during engine update
	void MakeCurrent();
	//Returns if scene is current
	bool IsCurrent();
	//Start() should be called once before update
	void Start();
	//Should be called every frame (called in default NWengine update NWengine normally)
	void Update();
	//Sets up needed static variables and other things
	void SetUp(); 
	//Draws all scene objs (called within default NWengine update normally)
	void Draw();
	//Serialize the scene and save it in a file with the same name
	void Save();
	//This static method calls Update on current scene if not null
	//Changes "name" member to given path.
	void SetPath(const std::string& path);

	//Create new scene and push it to a container
	static Scene& CreateNew(const std::string& path);
	//Gets a scene, given its path. If multiple scenes have same path it returns one of them; order or determinism isn't garunteed
	static Scene* GetScene(const std::string& path);
	//Deletes the scene having a certain path. Return value is whether the scene is found or not
	static bool   DeleteScene(const std::string& path);
	static void   Destroy();
	static void   UpdateActiveScene();
	static Scene* GetCurrent();

	static std::list<Scene> _scenes; 
	static Scene* currentScene;
};