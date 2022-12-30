#pragma once
#include "GameObject.h"

class AudioListener : public GameComponent {
public:
	static std::string GetType() { return "AudioListener"; };
	AudioListener() {};
	~AudioListener();
	
	AudioListener(GameObject* attachedObj);
	GameObject* attachedObj = nullptr;
	static std::map<GameObject*, AudioListener*> componentList;

	void Update();

};