#include "Audio.h"
#include "AudioLib.h"
#include "Globals.h"

bool Sound::Init() { 
    bool res = false; 
    NW_AL_CALL(res = AudioLib::createCtx())
    return !res; 
}

void Sound::Destroy() { 
    NW_AL_CALL_VOID(AudioLib::destroyCtx()); 
}

Asset* Sound::GetFromCache(void* id) {
	auto iter = resList.find(*(SoundIdentifier*)id);
	if(iter == resList.end())
		return nullptr;
	return &iter->second;
}

Asset* Sound::LoadFromFile(const char* path, void* id) {
	AudioLib::DataBuffer buffer; 
    NW_AL_CALL(buffer = AudioLib::genBuff());
	NW_AL_CALL(AudioLib::loadWav(path, buffer));
	return LoadFromBuffer(buffer, id);
}

Asset* Sound::LoadFromBuffer(void* alBuffer, void* id) {
	Sound& snd	= resList.emplace(*(SoundIdentifier*)id, Sound()).first->second;
	snd._buffID = alBuffer;
	NW_AL_CALL(AudioLib::genSrc(&snd._source, snd._buffID));
	return &snd;
}

NW_IMPL_RES_LIST(SoundIdentifier, Sound)

void Sound::Clean() {
	--_usageCounter;
	if(_usageCounter > 0)
		return;
	SoundIdentifier id = GetIDWithAsset<Sound*, SoundIdentifier>(this);
	this->Stop();
	NW_AL_CALL_VOID(AudioLib::delSrc(_source));
	NW_AL_CALL_VOID(AudioLib::delBuff(_buffID));
	EraseRes<Sound>(id);
}

void Sound::Play() {
	if(this->isPlaying)
		return;
	AudioLib::SourceInitData initData;
	initData.buff	   = _buffID;
	initData.loopTimes = isLooping ? NW_AUD_LOOP_INFINITE : NW_AUD_LOOP_DONT;
	NW_AL_CALL(AudioLib::initSrc(_source, initData));
	NW_AL_CALL(AudioLib::playSrc(_source));
	this->isPlaying = 1;
	_hasFinished	= 0;
}

void Sound::Pause() {
	if(!isPlaying)
		return;
	_isPaused = 1;
	NW_AL_CALL(AudioLib::pauseSrc(_source));
}

void Sound::Resume() {
	if(!_isPaused)
		return;
	_isPaused = 0;
	NW_AL_CALL(AudioLib::playSrc(_source));
}

void Sound::Stop() {
	if(!this->isPlaying)
		return;
	NW_AL_CALL(AudioLib::stopSrc(_source));
	this->isPlaying	   = 0;
	this->_hasFinished = 1;
}

void Sound::Reset() {
	Stop();
	_hasFinished = 0;
}

void Sound::SetVolume(float volume) {
	if(volume == this->volume)
		return;
	this->volume = volume;
	NW_AL_CALL(AudioLib::setSrcVol(_source, this->volume)); // TODO::Add error report if this fails
}

bool Sound::HasFinished() { return _hasFinished; }

bool Sound::_HasFinished() {
	AudioLib::SourceInfo info;
	NW_AL_CALL_VOID(AudioLib::querySrcInfo(_source, &info));
	return !info.isPlaying;
}

void Sound::SetFrequency(float frequency) {
	frequency = Min<float>(frequency, 2);
	if(frequency == this->frequency)
		return;
	this->frequency = Clamp<float>(frequency, 0.0f, 2.0f);
	NW_AL_CALL(AudioLib::setSrcPitch(_source, this->frequency)); // TODO::Add error report if this fails
}

void Sound::SetLoop(bool loop) { isLooping = loop; }
