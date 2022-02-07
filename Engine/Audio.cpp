#include<sndfile.h>
#include<vector>
#include "Audio.h"
bool InitOpenAL(){
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

ALuint LoadSound(const char* path) {
	SF_INFO info;
	SNDFILE* data = sf_open(path, SFM_READ, &info);  //hold on, does not support mp3? TODO::Write your own sound loading solution
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
	//TODO::error checking
	return buffer;
}