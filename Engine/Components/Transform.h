#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"

#include "Console.h"
class Transform : public GameComponent {
public:
	static std::string GetType() { return "Transform"; };

	fVec2 position = fVec2(0.0f,0.0f);
	fVec2 scale    = fVec2(1.0f, 1.0f);
	float rotation = 0.0f;
	GameObject* attachedObj;
	Transform() {};
	Transform(GameObject* go);

	void Gui() override;
	int Serialize(std::fstream* data, int offset) override;
	int Deserialize(std::fstream* data, int offset) override;

	void  SetGameObject(void* go)  override;
	void* GetGameObject()		   override;
	static std::map<GameObject*, Transform> componentList;
};
