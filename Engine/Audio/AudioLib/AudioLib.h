#pragma once
#include <typeinfo>

#define SWAP_ENDIAN_16(v)  (v << 8  | v >> 8)
#define SWAP_ENDIAN_32(v)  (v << 24 | v >> 24 | (v << 8 & 0x00FF0000) | (v >> 8 & 0x0000FF00))

#define NW_AUD_ERROR                       -69
#define NW_AUD_ERROR_INITIALIZATION_FAILED -1
#define NW_AUD_ERROR_FILE_OPEN 1
#define NW_AUD_CORRUPTED_WAVE  2
#define NW_AUD_NOT_PCM         3 

#define NW_AUD_WIN_CHECK(a, errorCode) { if((a) != 0) { return errorCode; } }

namespace  AudioLib {

typedef uint16_t uint16;
typedef uint16_t uint32;
typedef void*    DataBuffer;
typedef void*    Source;

#define NW_AUD_LOOP_INFINITE -1
#define NW_AUD_LOOP_DONT      0
struct SourceInitData {
    DataBuffer buff;
    int loopTimes = NW_AUD_LOOP_DONT;
};

struct SourceInfo {
    bool isPlaying = 0;    
};

DataBuffer genBuff();

void delBuff(DataBuffer);

int genSrc(Source* srcOut, DataBuffer waveData);

void delSrc(Source);

int createCtx();

void  destroyCtx();

void* getCtx();

int  initSrc(Source, const SourceInitData&);

int playSrc(Source);

int stopSrc(Source);

void setSrcVol(Source, float);

void setSrcPitch(Source, float i);

void querySrcInfo(Source, SourceInfo*);
/*
  Only PCM is supported.
*/
int loadWav(const char* path, DataBuffer outData);

};
