#include "AudioEmitter.h"
#include "Utilities.h"
#include "imgui/imgui.h"

std::map<GameObject*, AudioEmitter*> AudioEmitter::componentList;

AudioEmitter::AudioEmitter(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
	AudioEmitter::componentList.insert(std::make_pair(attachedObj, this));
}

AudioEmitter::~AudioEmitter() {
	delete this->sound;
	this->sound = nullptr;
	AudioEmitter::componentList.erase(this->attachedObj);
}


int AudioEmitter::Serialize(std::fstream* data, int offset)	{
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "AudioEmitter", 12, sizeBuffer);

	if (this->sound == nullptr) { WRITE_ON_BIN(data, "", 0, sizeBuffer);}
	else { WRITE_ON_BIN(data, this->sound->name.c_str(), this->sound->name.size(), sizeBuffer); }

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
		this->sound = new Sound(temp);
	}
	delete[] temp;

	//Deserializing sound parameters
	READ_FROM_BIN(data, &this->volume, sizeBuffer);
	READ_FROM_BIN(data, &this->frequency, sizeBuffer);
	READ_FROM_BIN(data, &this->isLooping, sizeBuffer);
	return 0;
};


NW_IMPL_GUI( AudioEmitter,
	std::string temp = "";
	if (sound != nullptr) temp = sound->name;

	if (NWGui::FileHolder("Sound", temp)) {
		std::string path = GetFile(WIN_STR_FILTER("Sound Files", "*.wav;*.ogg;*.flac"));
		if (path == "") return;

		if (sound == nullptr) { sound = new Sound(path); return; } //TODO::Add Sound function
		if (sound->name == path) return;
		delete sound;
		sound = new Sound(path);
		//TODO::Check only the file name
	}

	if (sound == nullptr) return;
	bool temp0 = sound->isPlaying;
	if (NWGui::CheckBox("isPlaying", &temp0 ) ) {
		if (!sound->isPlaying)
			sound->Play();
		else
			sound->Stop();
		temp0 = !temp0;
	}

	NWGui::DragValue("Volume", &this->volume, ImGuiDataType_S32, 1, 1, 0.0f, 100.0f);
	NWGui::DragValue("Frequency", &this->frequency, ImGuiDataType_Float, 1, 0.1f, 0.0f, 2.0f);
	NWGui::CheckBox("Loop", &this->isLooping);
)
