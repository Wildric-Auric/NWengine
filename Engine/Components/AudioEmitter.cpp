#include "AudioEmitter.h"

std::map<GameObject*, AudioEmitter*> AudioEmitter::componentList;

bool AudioEmitter::ConditionHasAudioEmitter(GameObject* obj) {
    AudioEmitter* ae = obj->GetComponent<AudioEmitter>();
    return ae;
}

AudioEmitter::AudioEmitter() {}

AudioEmitter::AudioEmitter(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
	AudioEmitter::componentList.insert(std::make_pair(attachedObj, this));
}

AudioEmitter::~AudioEmitter() {
	if (sound != nullptr) {
		sound->Clean();
	}
	AudioEmitter::componentList.erase(this->attachedObj);
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

void AudioEmitter::SetSound(const Sound* snd) {
	if (sound != nullptr)
		sound->Clean();

	Sound loader;
	SoundIdentifier id = { "", (uint64)this};
	sound = (Sound*)loader.LoadFromBuffer((void*)&snd->_buffID, &id);
}

void AudioEmitter::StopIfHasFinished() {
    if (sound->isPlaying && sound->HasFinished())
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

int AudioEmitter::Serialize(std::fstream* data, int offset)	{
	SoundIdentifier id = GetIDWithAsset<Sound*, SoundIdentifier>(sound);

	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "AudioEmitter", 12, sizeBuffer);

	if (this->sound == nullptr ) { WRITE_ON_BIN(data, "", 0, sizeBuffer);}
	else { WRITE_ON_BIN(data, id.path.c_str(), id.path.size(), sizeBuffer); }

	WRITE_ON_BIN(data, &this->volume,    sizeof(this->volume)  , sizeBuffer);
	WRITE_ON_BIN(data, &this->frequency, sizeof(this->frequency), sizeBuffer);
	WRITE_ON_BIN(data, &this->isLooping, sizeof(this->isLooping), sizeBuffer);
	return 0; //TODO::Serialize sound
};

int AudioEmitter::Deserialize(std::fstream* data, int offset) {
	int  sizeBuffer = 0;
	//Deserializing sound
	char* temp = new char[512];
	READ_FROM_BIN(data, temp, sizeBuffer);
	temp[sizeBuffer] = '\0';
	if (strlen(temp) != 0) {
		Sound loader;
		SoundIdentifier sndID{ temp, (uint64)this};
		this->sound = (Sound*)loader.LoadFromFile(temp, &sndID);
	}
	delete[] temp;

	//Deserializing sound parameters
	READ_FROM_BIN(data, &this->volume, sizeBuffer);
	READ_FROM_BIN(data, &this->frequency, sizeBuffer);
	READ_FROM_BIN(data, &this->isLooping, sizeBuffer);
	return 0;
};


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
