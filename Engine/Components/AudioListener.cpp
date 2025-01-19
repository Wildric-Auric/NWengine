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
    Scene* s = Scene::GetCurrent();

    auto proc = [](GameObject* go, void* data) -> int {
		AudioEmitter* audioEmitter = go->GetComponent<AudioEmitter>();
		float volume    = (float)(audioEmitter->volume);
		float freq      = audioEmitter->frequency;	
		audioEmitter->SetVolume(volume);
		audioEmitter->SetFrequency(freq);
		audioEmitter->SetLooping(audioEmitter->isLooping);
        audioEmitter->StopIfHasFinished();
        return 1;
    };

    s->CacheMap(AudioEmitter::ConditionHasAudioEmitter, proc, nullptr);
}

int AudioListener::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "AudioListener", 13, sizeBuffer);
	return 0;
}
int AudioListener::Deserialize(std::fstream* data, int offset) {
	return 0;
}

