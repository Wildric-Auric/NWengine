#include "AudioListener.h"
#include "AudioEmitter.h"
#include "Scene.h"

std::map<GameObject*, AudioListener*>AudioListener::componentList;

AudioListener::AudioListener(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
	this->componentList.insert(std::make_pair(attachedObj, this));
}

AudioListener::~AudioListener() {
	AudioListener::componentList.erase(this->attachedObj);
}


//TODO::All calculation for distance and direction for sound fading etc
void AudioListener::Update() {
	for (auto iter = Scene::currentScene->sceneObjs.begin(); iter != Scene::currentScene->sceneObjs.end(); iter++) {
		GameObject* go = &(*iter);
		AudioEmitter* audioEmitter = go->GetComponent<AudioEmitter>();
		if (audioEmitter == nullptr) continue;
		if (!audioEmitter->sound) continue;
		float volume    = (float)(audioEmitter->volume)/100.0f;
		float freq      = audioEmitter->frequency;	

		audioEmitter->sound->SetVolume(volume);
		audioEmitter->sound->SetFrequency(freq);
		audioEmitter->sound->SetLoop(audioEmitter->isLooping);

		if (audioEmitter->sound->isPlaying) {
			if (audioEmitter->sound->HasFinished()) {
				audioEmitter->sound->Stop();
			}
		}
	}
}

int AudioListener::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "AudioListener", 13, sizeBuffer);
	return 0;
}
int AudioListener::Deserialize(std::fstream* data, int offset) {
	return 0;
}

