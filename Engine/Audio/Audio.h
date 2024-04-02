#pragma once
#include <string>
#include <unordered_map>
#include "Globals.h"
#include "Asset.h"


struct SoundIdentifier {
	std::string path;
	uint64 runtimeID;	

	bool operator==(const SoundIdentifier& other) const {
		return path == other.path && runtimeID == other.runtimeID;
	}
};


template <>
struct std::hash<SoundIdentifier>
{
	std::size_t operator()(const SoundIdentifier& t) const
	{
		std::size_t res = 0;
		hashCombine(res, t.path);
		hashCombine(res, t.runtimeID);
		return res;
	}
};

//TODO::Make this to avoid duplicate buffers
//struct SoundBuffer {
//	
//	counter;
//};



class Sound : Asset {
public:
	uint32 _buffID = 0;
	uint32 _source = 0;
	
	float volume    = 1.0f;
	float frequency = 1.0f;
	bool  isLooping = 0;
	bool  isPlaying = 0;

	Sound() = default;
	void Play();
	void SetVolume(float volume);
	void SetFrequency(float frequency);
	void SetLoop(bool loop);
	void Stop();
	bool HasFinished();

	void Delete();

	Asset* GetFromCache(void* id);
	Asset* LoadFromFile(const char* path, void* id);
	Asset* LoadFromBuffer(void* alBuffer, void* id);

	NW_DECL_RES_LIST(SoundIdentifier, Sound);
	static bool Init();
	static void Destroy();

};

