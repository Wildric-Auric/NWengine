#include "AudioEmitter.h"
#include "Scene.h"


bool AudioEmitter::ConditionHasAudioEmitter(GameObject* obj) {
    AudioEmitter* ae = obj->GetComponent<AudioEmitter>();
    return ae;
}

void AudioEmitter::OnAdd() {
	Scene::GetCurrent()->AddToCache(AudioEmitter::ConditionHasAudioEmitter, *attachedObject);
}

void AudioEmitter::OnDelete() {
	Scene::GetCurrent()->DeleteFromCache(AudioEmitter::ConditionHasAudioEmitter, *attachedObject);
}

AudioEmitter::AudioEmitter() {}

AudioEmitter::AudioEmitter(GameObject* obj) {
	this->attachedObject = obj;
}

AudioEmitter::~AudioEmitter() {
	if (sound != nullptr) {
		sound->Clean();
	}
}

void AudioEmitter::SetSound(const std::string& path) {
	if (sound != nullptr) {
		sound->Clean();
	}
	Sound loader;
	SoundIdentifier id = {path, (uint64)this};
	sound = (Sound*)loader.LoadFromFile(path.c_str(), &id); //TODO::Make buffer shared across multiple 'sounds'
}

void AudioEmitter::Play() {
    sound->Play();
}

void AudioEmitter::Stop() {
    sound->Stop();
}

void AudioEmitter::Pause() {
	sound->Pause();
}

void AudioEmitter::Resume() {
	sound->Resume();
}

void AudioEmitter::SetSound(const Sound* snd) {
	if (sound != nullptr)
		sound->Clean();

	Sound loader;
	SoundIdentifier id = { "", (uint64)this};
	sound = (Sound*)loader.LoadFromBuffer((void*)&snd->_buffID, &id);
}

void AudioEmitter::StopIfHasFinished() {
    if (sound->isPlaying && sound->_HasFinished())
        Stop();
}

void AudioEmitter::SetLooping(bool v) {
    isLooping = v;
    sound->SetLoop(v);
}

void AudioEmitter::SetFrequency(float v) {
    frequency = v;
    sound->SetFrequency(frequency);
}

void AudioEmitter::SetVolume(float v) {
    volume = v;
    sound->SetVolume(volume / 100.0f);
}

GameObject* MultiAudioEmitter::AddEmitter() {
    GameObject*   obj = new GameObject(); 
    AudioEmitter* em  =obj->AddComponent<AudioEmitter>();
    _container.emplace(obj, em);
    return obj;
}

GameObject* MultiAudioEmitter::GetEmitterIf(bool(*cond)(GameObject*)) {
    for (auto key : _container) {
        if (cond(key.first)) return key.first;
    }
    return nullptr;
}

void MultiAudioEmitter::DeleteEmitter(GameObject* element) {
    auto iter = _container.find(element);
    if (iter == _container.end()) return;
    iter->first->DeleteComponents();
    delete iter->first;
    _container.erase(iter);
}

AudioEmitter* MultiAudioEmitter::GetEmitterAudioIf(bool(*cond)(AudioEmitter*)) {
    for (auto key : _container) {
        if (cond(key.second)) return key.second;
    }
    return nullptr;
}

AudioEmitter* MultiAudioEmitter::GetEmitterAudio(GameObject* obj) {
    auto iter = _container.find(obj);
    if (iter == _container.end()) return nullptr;
    return iter->second;
}

MultiAudioEmitter::~MultiAudioEmitter() {
    for (auto key : _container) {
        key.first->DeleteComponents();
        delete key.first;
    }
}
