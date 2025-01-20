#include "AudioListener.h"
#include "AudioEmitter.h"
#include "Scene.h"

AudioListener::AudioListener(GameObject* obj) {
	this->attachedObject = obj;
}

AudioListener::~AudioListener() {

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
