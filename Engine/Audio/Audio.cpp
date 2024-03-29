#include<sndfile.h>
#include<vector>
#include "Audio.h"
#include "Globals.h"
#include <AL/al.h>
#include <AL/alc.h>


bool SoundSystem::InitOpenAL() {
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


void SoundSystem::DestroyOpenAL() {
	ALCcontext* ctx			= alcGetCurrentContext();
	ALCdevice*  device		= alcGetContextsDevice(ctx);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);
	alcCloseDevice(device);
}


uint32 SoundSystem::LoadSound(const char* path) {
	SF_INFO info;
	SNDFILE* data = sf_open(path, SFM_READ, &info);  //hold on, does not support mp3? TODO::Write your own sound loading solution
	if (!data) { printf("ERROR::Could not open sound file at : %s", path); return 0; }
	ALsizei sampleNumber = static_cast<ALsizei>(info.channels * info.frames);
	ALsizei sampleRate = static_cast<ALsizei>(info.samplerate);
	std::vector<ALshort> samples(sampleNumber);
	if (sf_read_short(data, &samples[0], sampleNumber) < sampleNumber) {
		printf("Need to read doc?");
		return 0;
	}
	sf_close(data);
	ALenum format = 0; //MONO8, STEREO8, MONO16, STEREO16
	if (info.channels == 1) format = AL_FORMAT_MONO16;
	else if (info.channels == 2) format = AL_FORMAT_STEREO16;

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, &samples[0], sampleNumber * sizeof(ALushort), sampleRate);
	ALenum err = alGetError();
	if (err == AL_NO_ERROR) 
		return buffer;
	NW_LOG_ERROR((std::string("OpenAL Error: ") + std::to_string(err)).c_str());
	return 0;
}

Sound::Sound(std::string path) {
	this->snd = SoundSystem::LoadSound(path.c_str());
	name	  = path;
	if (!this->snd) { return; }
	alGenSources(1, &this->source);
	alSourcei(source, AL_BUFFER, this->snd);
}

Sound::~Sound() {
	this->Stop();
	alSourcei(this->source, AL_BUFFER, 0);
	alDeleteSources(1, &this->source);
	alDeleteBuffers(1, &this->snd);
}

void Sound::Play() {
	if (this->isPlaying) return;
	alSourcePlay(source);
	this->isPlaying		= 1;
}

void Sound::Stop() {
	if (!this->isPlaying) return;
	alSourceStop(source);
	this->isPlaying = 0;
}

void Sound::SetVolume(float volume) {
	if (volume == this->volume) return;
	this->volume = volume; 
	alSourcef(source, AL_GAIN, this->volume);
}

bool Sound::HasFinished() {
	int state = 0;
	alGetSourcei(this->source, AL_SOURCE_STATE, &state);
	return (state != AL_PLAYING);
}
//Not Written in the doc; but max value of pitch is 2.0 otherwise error
void Sound::SetFrequency(float frequency) {
	if (frequency == this->frequency) return;
	this->frequency = Clamp<float>(frequency, 0.0f, 2.0f);
	alSourcef(source, AL_PITCH, this->frequency);
}

void Sound::SetLoop(bool loop) {
	if (loop == this->isLooping) return;
	this->isLooping = loop;
	alSourcei(source, AL_LOOPING, this->isLooping);
}



