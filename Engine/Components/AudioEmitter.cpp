#include "AudioEmitter.h"


std::map<GameObject*, AudioEmitter*> AudioEmitter::componentList;

AudioEmitter::AudioEmitter(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
	AudioEmitter::componentList.insert(std::make_pair(attachedObj, this));
}

AudioEmitter::~AudioEmitter() {
	delete this->sound;
	AudioEmitter::componentList.erase(this->attachedObj);

}