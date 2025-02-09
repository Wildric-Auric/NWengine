#pragma once
#include <typeinfo>

#define SWAP_ENDIAN_16(v)  (v << 8  | v >> 8)
#define SWAP_ENDIAN_32(v)  (v << 24 | v >> 24 | (v << 8 & 0x00FF0000) | (v >> 8 & 0x0000FF00))

#define NW_AUD_ERROR                       69
#define NW_AUD_ERROR_INITIALIZATION_FAILED -1
#define NW_AUD_ERROR_FILE_OPEN 1
#define NW_AUD_CORRUPTED_WAVE  2
#define NW_AUD_NOT_PCM         3 

#define NW_AUD_WIN_CHECK(a, errorCode) { if((a) != 0) { return errorCode; } }


typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint8_t BYTE;
typedef int BOOL;
//-----------Fetched from xaudio2fx header--------------
typedef struct AudioLib_REVERB_PARAMETERS
{
    // ratio of wet (processed) signal to dry (original) signal
    float WetDryMix;            // [0, 100] (percentage)

    // Delay times
    UINT32 ReflectionsDelay;    // [0, 300] in ms
    BYTE ReverbDelay;           // [0, 85] in ms
    BYTE RearDelay;             // 7.1: [0, 20] in ms, all other: [0, 5] in ms
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN10)
    BYTE SideDelay;             // 7.1: [0, 5] in ms, all other: not used, but still validated
#endif


    // Indexed parameters
    BYTE PositionLeft;          // [0, 30] no units
    BYTE PositionRight;         // [0, 30] no units, ignored when configured to mono
    BYTE PositionMatrixLeft;    // [0, 30] no units
    BYTE PositionMatrixRight;   // [0, 30] no units, ignored when configured to mono
    BYTE EarlyDiffusion;        // [0, 15] no units
    BYTE LateDiffusion;         // [0, 15] no units
    BYTE LowEQGain;             // [0, 12] no units
    BYTE LowEQCutoff;           // [0, 9] no units
    BYTE HighEQGain;            // [0, 8] no units
    BYTE HighEQCutoff;          // [0, 14] no units

    // Direct parameters
    float RoomFilterFreq;       // [20, 20000] in Hz
    float RoomFilterMain;       // [-100, 0] in dB
    float RoomFilterHF;         // [-100, 0] in dB
    float ReflectionsGain;      // [-100, 20] in dB
    float ReverbGain;           // [-100, 20] in dB
    float DecayTime;            // [0.1, inf] in seconds
    float Density;              // [0, 100] (percentage)
    float RoomSize;             // [1, 100] in feet

    // component control
    BOOL DisableLateField;      // TRUE to disable late field reflections
} AudioLib_REVERB_PARAMETERS;

struct AudioLib_REVERB_I3DL2_PARAMETERS
{
    // ratio of wet (processed) signal to dry (original) signal
    float WetDryMix;            // [0, 100] (percentage)

    // Standard I3DL2 parameters
    INT32 Room;                 // [-10000, 0] in mB (hundredths of decibels)
    INT32 RoomHF;               // [-10000, 0] in mB (hundredths of decibels)
    float RoomRolloffFactor;    // [0.0, 10.0]
    float DecayTime;            // [0.1, 20.0] in seconds
    float DecayHFRatio;         // [0.1, 2.0]
    INT32 Reflections;          // [-10000, 1000] in mB (hundredths of decibels)
    float ReflectionsDelay;     // [0.0, 0.3] in seconds
    INT32 Reverb;               // [-10000, 2000] in mB (hundredths of decibels)
    float ReverbDelay;          // [0.0, 0.1] in seconds
    float Diffusion;            // [0.0, 100.0] (percentage)
    float Density;              // [0.0, 100.0] (percentage)
    float HFReference;          // [20.0, 20000.0] in Hz

    AudioLib_REVERB_I3DL2_PARAMETERS(
        float wetDryMix, INT32 room, INT32 roomHF, float roomRolloffFactor, float decayTime,
        float decayHFRatio, INT32 reflections, float reflectionsDelay, INT32 reverb,
        float reverbDelay, float diffusion, float density, float hfReference)
    {
        WetDryMix = wetDryMix;
        Room = room;
        RoomHF = roomHF;
        RoomRolloffFactor = roomRolloffFactor;
        DecayTime = decayTime;
        DecayHFRatio = decayHFRatio;
        Reflections = reflections;
        ReflectionsDelay = reflectionsDelay;
        Reverb = reverb;
        ReverbDelay = reverbDelay;
        Diffusion = diffusion;
        Density = density;
        HFReference = hfReference;
    }
};

#define AudioLib_I3DL2_PRESET_DEFAULT         AudioLib_REVERB_I3DL2_PARAMETERS( 100,-10000,    0,0.0f, 1.00f,0.50f,-10000,0.020f,-10000,0.040f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_GENERIC         AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -100,0.0f, 1.49f,0.83f, -2602,0.007f,   200,0.011f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_PADDEDCELL      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-6000,0.0f, 0.17f,0.10f, -1204,0.001f,   207,0.002f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_ROOM            AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -454,0.0f, 0.40f,0.83f, -1646,0.002f,    53,0.003f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_BATHROOM        AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-1200,0.0f, 1.49f,0.54f,  -370,0.007f,  1030,0.011f,100.0f, 60.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_LIVINGROOM      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-6000,0.0f, 0.50f,0.10f, -1376,0.003f, -1104,0.004f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_STONEROOM       AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -300,0.0f, 2.31f,0.64f,  -711,0.012f,    83,0.017f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_AUDITORIUM      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -476,0.0f, 4.32f,0.59f,  -789,0.020f,  -289,0.030f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_CONCERTHALL     AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -500,0.0f, 3.92f,0.70f, -1230,0.020f,    -2,0.029f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_CAVE            AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,    0,0.0f, 2.91f,1.30f,  -602,0.015f,  -302,0.022f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_ARENA           AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -698,0.0f, 7.24f,0.33f, -1166,0.020f,    16,0.030f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_HANGAR          AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-1000,0.0f,10.05f,0.23f,  -602,0.020f,   198,0.030f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_CARPETEDHALLWAY AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-4000,0.0f, 0.30f,0.10f, -1831,0.002f, -1630,0.030f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_HALLWAY         AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -300,0.0f, 1.49f,0.59f, -1219,0.007f,   441,0.011f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_STONECORRIDOR   AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -237,0.0f, 2.70f,0.79f, -1214,0.013f,   395,0.020f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_ALLEY           AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -270,0.0f, 1.49f,0.86f, -1204,0.007f,    -4,0.011f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_FOREST          AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-3300,0.0f, 1.49f,0.54f, -2560,0.162f,  -613,0.088f, 79.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_CITY            AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -800,0.0f, 1.49f,0.67f, -2273,0.007f, -2217,0.011f, 50.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_MOUNTAINS       AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-2500,0.0f, 1.49f,0.21f, -2780,0.300f, -2014,0.100f, 27.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_QUARRY          AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-1000,0.0f, 1.49f,0.83f,-10000,0.061f,   500,0.025f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_PLAIN           AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-2000,0.0f, 1.49f,0.50f, -2466,0.179f, -2514,0.100f, 21.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_PARKINGLOT      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,    0,0.0f, 1.65f,1.50f, -1363,0.008f, -1153,0.012f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_SEWERPIPE       AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-1000,0.0f, 2.81f,0.14f,   429,0.014f,   648,0.021f, 80.0f, 60.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_UNDERWATER      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000,-4000,0.0f, 1.49f,0.10f,  -449,0.007f,  1700,0.011f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_SMALLROOM       AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -600,0.0f, 1.10f,0.83f,  -400,0.005f,   500,0.010f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_MEDIUMROOM      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -600,0.0f, 1.30f,0.83f, -1000,0.010f,  -200,0.020f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_LARGEROOM       AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -600,0.0f, 1.50f,0.83f, -1600,0.020f, -1000,0.040f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_MEDIUMHALL      AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -600,0.0f, 1.80f,0.70f, -1300,0.015f,  -800,0.030f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_LARGEHALL       AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -600,0.0f, 1.80f,0.70f, -2000,0.030f, -1400,0.060f,100.0f,100.0f,5000.0f)
#define AudioLib_I3DL2_PRESET_PLATE           AudioLib_REVERB_I3DL2_PARAMETERS( 100, -1000, -200,0.0f, 1.30f,0.90f,     0,0.002f,     0,0.010f,100.0f, 75.0f,5000.0f)
//-------------------------




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

int createCtx(bool enableLog = 1);

void  destroyCtx();

void* getCtx();

void* getDefMaster();

int setDefMasterVol(float);

int initSrc(Source, const SourceInitData&);

int playSrc(Source);

int stopSrc(Source);

int pauseSrc(Source);

int setSrcVol(Source, float);

int setSrcPitch(Source, float i);

int addReverb(Source, const AudioLib_REVERB_PARAMETERS&);

int addReverb(Source, const AudioLib_REVERB_I3DL2_PARAMETERS&);

int setReverbParams(Source, const AudioLib_REVERB_PARAMETERS&);

int setReverbParams(Source, const AudioLib_REVERB_I3DL2_PARAMETERS&); 

int enableReverb(Source);

int disableReverb(Source);

int i3dl2Native(const AudioLib_REVERB_I3DL2_PARAMETERS& in, AudioLib_REVERB_PARAMETERS* out);

void querySrcInfo(Source, SourceInfo*);
/*
  Only PCM is supported.
*/
int loadWav(const char* path, DataBuffer outData);

};
