#pragma once
#include "GameObject.h"
#include "Audio.h"
class AudioEmitter : public GameComponent {
public:
	static std::string GetType() { return "AudioEmitter"; };
	AudioEmitter() {};
	AudioEmitter(GameObject* attachedObj);
	~AudioEmitter();
	GameObject* attachedObj = nullptr;
	static std::map <GameObject*, AudioEmitter*> componentList;

	Sound sound;
	int			volume    = 100; //Should be clamped from 0.0 to 1.0 in order to be past to Sound.setvolume
	float 		frequency = 1.0f;
	bool        isLooping = 0;
};