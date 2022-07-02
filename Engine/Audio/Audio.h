#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <string>

bool InitOpenAL();

ALuint LoadSound(const char* path);


class Sound {
private:
	ALuint snd;
	ALuint source;
public:
	float volume = 1.0f;
	float frequency;
	bool isLooping = 0;

	Sound(std::string path);
	void Play();
	void SetVolume(float volume);
	void SetFrequency(float frequency);
	void SetLoop(bool loop);
	void Stop();

};


