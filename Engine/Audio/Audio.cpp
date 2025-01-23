#include<sndfile.h>
#include<vector>
#include "Audio.h"
#include "Globals.h"
#include <AL/al.h>
#include <AL/alc.h>

void* Sound::_device = 0;

bool Sound::Init() {
	ALCdevice * device = alcOpenDevice(0);
	if (!device)
		return false;
 
	ALCcontext* Context = alcCreateContext(device, 0);
	if (!Context)
		return false;

	if (!alcMakeContextCurrent(Context))
		return false;

    _device = device;
	return true;
}


void Sound::Destroy() {
	ALCcontext* ctx			= NW_AL_CALL(alcGetCurrentContext());
	ALCdevice*  device		= NW_AL_CALL(alcGetContextsDevice(ctx));
	NW_AL_CALL(alcMakeContextCurrent(NULL));
	NW_AL_CALL(alcDestroyContext(ctx));
	NW_AL_CALL(alcCloseDevice(device));
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
		printf("Need to read doc?\n");
		return nullptr;
	}
	sf_close(data);
	ALenum format = 0; //MONO8, STEREO8, MONO16, STEREO16
	if (info.channels == 1) format = AL_FORMAT_MONO16;
	else if (info.channels == 2) format = AL_FORMAT_STEREO16;

	ALuint buffer;
	NW_AL_CALL(alGenBuffers(1, &buffer));
	alBufferData(buffer, format, &samples[0], sampleNumber * sizeof(ALushort), sampleRate);
	ALenum err = alGetError();
	if (err != AL_NO_ERROR) {
		NW_LOG_ERROR((std::string("OpenAL Error during loading: ") + std::to_string(err) + " " + std::to_string(sampleNumber) + " " 
                            + std::to_string(samples[0]) + " " + std::to_string(format)).c_str()); //TODO::Delete buffers here
		return nullptr;
	}
	return LoadFromBuffer(&buffer, id);
}

Asset* Sound::LoadFromBuffer(void* alBuffer, void* id) {
	Sound&	snd = resList.emplace(*(SoundIdentifier*)id, Sound()).first->second;
	snd._buffID = *(ALuint*)alBuffer;
	NW_AL_CALL(alGenSources(1, &snd._source));
	NW_AL_CALL(alSourcei(snd._source, AL_BUFFER, snd._buffID));
	return &snd;
}

NW_IMPL_RES_LIST(SoundIdentifier, Sound)


void Sound::Clean() {
	--_usageCounter;
	if (_usageCounter > 0)
		return;
	SoundIdentifier id = GetIDWithAsset<Sound*,SoundIdentifier>(this);
	this->Stop();
	NW_AL_CALL(alSourcei(this->_source, AL_BUFFER, 0));
	NW_AL_CALL(alDeleteSources(1, &this->_source));
	NW_AL_CALL(alDeleteBuffers(1, &this->_buffID));
	EraseRes<Sound>(id);
}

void Sound::Play() {
	if (this->isPlaying) return;
	NW_AL_CALL(alSourcePlay(_source));
	this->isPlaying		= 1;
}

void Sound::Stop() {
	if (!this->isPlaying) return;
	NW_AL_CALL(alSourceStop(_source));
	this->isPlaying = 0;
}

void Sound::SetVolume(float volume) {
	if (volume == this->volume) return;
	this->volume = volume; 
	NW_AL_CALL(alSourcef(_source, AL_GAIN, this->volume));
}

bool Sound::HasFinished() {
	int state = 0;
	NW_AL_CALL(alGetSourcei(this->_source, AL_SOURCE_STATE, &state));
	return (state != AL_PLAYING);
}
//Not Written in the doc; but max value of pitch is 2.0 otherwise error
void Sound::SetFrequency(float frequency) {
	frequency =  Min<float>(frequency, 2);
	if (frequency == this->frequency) return;
	this->frequency = Clamp<float>(frequency, 0.0f, 2.0f);
	NW_AL_CALL(alSourcef(_source, AL_PITCH, this->frequency));
}

void Sound::SetLoop(bool loop) {
	if (loop == this->isLooping) return;
	this->isLooping = loop;
	NW_AL_CALL(alSourcei(_source, AL_LOOPING, this->isLooping));
}



