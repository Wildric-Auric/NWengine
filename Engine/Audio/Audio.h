#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <string>


class SoundSystem {
public:
	static bool	  InitOpenAL();
	static ALuint LoadSound(const char* path);
	static void   DestroyOpenAL();
};

class Sound {
private:
	ALuint snd = 0;
	ALuint source = 0;
public:
	std::string name = "";

	float volume    = 1.0f;
	float frequency = 1.0f;
	bool  isLooping = 0;
	bool  isPlaying = 0;

	Sound() {};
	Sound(std::string path);
	~Sound();
	void Play();
	void SetVolume(float volume);
	void SetFrequency(float frequency);
	void SetLoop(bool loop);
	void Stop();
	bool HasFinished();

};


