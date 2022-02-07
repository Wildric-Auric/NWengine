#pragma once

#include <AL/al.h>
#include <AL/alc.h>
bool InitOpenAL();

ALuint LoadSound(const char* path);