#include<sndfile.h>
#include<vector>
#include "Audio.h"
#include "Globals.h"
#include <AL/al.h>
#include <AL/alc.h>


bool Sound::Init() {
	ALCdevice * device = alcOpenDevice(0);
	if (!device)
		return false;

	ALCcontext* Context = alcCreateContext(device, 0);
	if (!Context)
		return false;

	if (!alcMakeContextCurrent(Context))
		return false;

	return true;
}


void Sound::Destroy() {
	ALCcontext* ctx			= alcGetCurrentContext();
	ALCdevice*  device		= alcGetContextsDevice(ctx);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);
	alcCloseDevice(device);
}

Asset* Sound::GetFromCache(void* id) {
	auto iter = resList.find(*(SoundIdentifier*)id);
	if (iter == resList.end()) 
		return nullptr;
	return &iter->second;
}

Asset* Sound::LoadFromFile(const char* path, void* id) {
	SF_INFO info;
	SNDFILE* data = sf_open(path, SFM_READ, &info);  //hold on, does not support mp3? TODO::Write your own sound loading solution
	if (!data) { printf("ERROR::Could not open sound file at : %s", path); return 0; }
	ALsizei sampleNumber = static_cast<ALsizei>(info.channels * info.frames);
	ALsizei sampleRate = static_cast<ALsizei>(info.samplerate);
	std::vector<ALshort> samples(sampleNumber);
	if (sf_read_short(data, &samples[0], sampleNumber) < sampleNumber) {
		printf("Need to read doc?");
		return nullptr;
	}
	sf_close(data);
	ALenum format = 0; //MONO8, STEREO8, MONO16, STEREO16
	if (info.channels == 1) format = AL_FORMAT_MONO16;
	else if (info.channels == 2) format = AL_FORMAT_STEREO16;

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, &samples[0], sampleNumber * sizeof(ALushort), sampleRate);
	ALenum err = alGetError();
	if (err != AL_NO_ERROR) {
		NW_LOG_ERROR((std::string("OpenAL Error: ") + std::to_string(err)).c_str()); //TODO::Delete buffers here
		return nullptr;
	}
	return LoadFromBuffer(&buffer, id);
}

Asset* Sound::LoadFromBuffer(void* alBuffer, void* id) {
	Sound&	snd = resList.emplace(*(SoundIdentifier*)id, Sound()).first->second;
	snd._buffID = *(ALuint*)alBuffer;
	alGenSources(1, &snd._source);
	alSourcei(snd._source, AL_BUFFER, snd._buffID);
	return &snd;
}

NW_IMPL_RES_LIST(SoundIdentifier, Sound)


void Sound::Delete() {
	SoundIdentifier id = GetIDWithAsset<Sound*,SoundIdentifier>(this);
	this->Stop();
	alSourcei(this->_source, AL_BUFFER, 0);
	alDeleteSources(1, &this->_source);
	alDeleteBuffers(1, &this->_buffID);
	resList.erase(id);
}

void Sound::Play() {
	if (this->isPlaying) return;
	alSourcePlay(_source);
	this->isPlaying		= 1;
}

void Sound::Stop() {
	if (!this->isPlaying) return;
	alSourceStop(_source);
	this->isPlaying = 0;
}

void Sound::SetVolume(float volume) {
	if (volume == this->volume) return;
	this->volume = volume; 
	alSourcef(_source, AL_GAIN, this->volume);
}

bool Sound::HasFinished() {
	int state = 0;
	alGetSourcei(this->_source, AL_SOURCE_STATE, &state);
	return (state != AL_PLAYING);
}
//Not Written in the doc; but max value of pitch is 2.0 otherwise error
void Sound::SetFrequency(float frequency) {
	frequency =  Min<float>(frequency, 2);
	if (frequency == this->frequency) return;
	this->frequency = Clamp<float>(frequency, 0.0f, 2.0f);
	alSourcef(_source, AL_PITCH, this->frequency);
}

void Sound::SetLoop(bool loop) {
	if (loop == this->isLooping) return;
	this->isLooping = loop;
	alSourcei(_source, AL_LOOPING, this->isLooping);
}



