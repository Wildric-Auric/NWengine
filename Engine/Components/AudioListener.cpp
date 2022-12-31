#include "AudioListener.h"
#include "AudioEmitter.h"

std::map<GameObject*, AudioListener*>AudioListener::componentList;

AudioListener::AudioListener(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
	this->componentList.insert(std::make_pair(attachedObj, this));
}

AudioListener::~AudioListener() {
	AudioListener::componentList.erase(this->attachedObj);
}


void AudioListener::Update() {
	for (std::map<GameObject*, AudioEmitter*>::iterator it = AudioEmitter::componentList.begin(); it != AudioEmitter::componentList.end(); it++) {
		AudioEmitter* audioEmitter = it->second;
		//TODO::All calculation for distance and direction for sound fading etc
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
