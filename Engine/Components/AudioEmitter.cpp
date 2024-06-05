#include "AudioEmitter.h"
#include "Utilities.h"

std::map<GameObject*, AudioEmitter*> AudioEmitter::componentList;

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

void AudioEmitter::SetSound(const Sound* snd) {
	if (sound != nullptr) {
		sound->Clean();
	}
	Sound loader;
	SoundIdentifier id = { "", (uint64)this};
	sound = (Sound*)loader.LoadFromBuffer((void*)&snd->_buffID, &id);
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