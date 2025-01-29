#include "AudioLib.h"
#include <commapi.h>
#include <xaudio2.h>
#include <vector>
#include <fstream>

namespace AudioLib {
    #define CAST(type, var) (type)var
    IXAudio2MasteringVoice* defaultMasterVoice = 0;
    IXAudio2*               ctx;

    struct WaveData {
        WAVEFORMATEX fmt{};
        std::vector<char*> data;
    };

    DataBuffer genBuff() {
        WaveData* ptr = new WaveData{};
        return (DataBuffer)(ptr);
    }

    void delBuff(DataBuffer buff) {
        delete (WaveData*)buff;
    }

    int genSrc(Source* srcOut, DataBuffer waveData) {
        IXAudio2SourceVoice* s = 0;
        WaveData* w = (WaveData*)waveData;
        NW_AUD_WIN_CHECK(ctx->CreateSourceVoice(&s, &w->fmt), NW_AUD_ERROR);
        *srcOut = s;
        return 0;    
    }
    
    int initSrc(Source src, const SourceInitData& data) {
        auto s = CAST(IXAudio2SourceVoice*, src);
        auto d = CAST(WaveData*, data.buff);
        XAUDIO2_BUFFER audBuff{};
        audBuff.pAudioData = (BYTE*)d->data.data();
        audBuff.AudioBytes = d->data.size();
        audBuff.LoopCount  = (data.loopTimes == NW_AUD_LOOP_DONT) ? 0: 
                             (data.loopTimes == NW_AUD_LOOP_INFINITE ?
                              XAUDIO2_LOOP_INFINITE : data.loopTimes);
        NW_AUD_WIN_CHECK(s->SubmitSourceBuffer(&audBuff), NW_AUD_ERROR);
        return 0;
    }

    void delSrc(Source src) { 
        auto s = CAST(IXAudio2SourceVoice*, src);
        s->DestroyVoice();
    }

    int  playSrc(Source src) {
        auto s = CAST(IXAudio2SourceVoice*, src);
		NW_AUD_WIN_CHECK(s->Start(), NW_AUD_ERROR);
        return 0;
    }

    int stopSrc(Source src) {
        auto s = CAST(IXAudio2SourceVoice*, src);
        NW_AUD_WIN_CHECK(s->Stop(), NW_AUD_ERROR);
        NW_AUD_WIN_CHECK(s->FlushSourceBuffers(), NW_AUD_ERROR);
        return 0;
    }

    int pauseSrc(Source src) {
        auto s = CAST(IXAudio2SourceVoice*, src);
        NW_AUD_WIN_CHECK(s->Stop(), NW_AUD_ERROR);
        return 0;
    }

    int setSrcVol(Source src, float i) {
        auto s = CAST(IXAudio2SourceVoice*, src);
        NW_AUD_WIN_CHECK(s->SetVolume(i), NW_AUD_ERROR);
        return 0;
    }

    int setSrcPitch(Source src, float i) {
        auto s = CAST(IXAudio2SourceVoice*, src);
        NW_AUD_WIN_CHECK(s->SetFrequencyRatio(i), NW_AUD_ERROR);
        return 0;
    }

    void querySrcInfo(Source src, SourceInfo* info) {
        auto s = CAST(IXAudio2SourceVoice*, src);
        XAUDIO2_VOICE_STATE state;
        s->GetState(&state);
        info->isPlaying = state.BuffersQueued > 0;
    }

    int createCtx() {
        HRESULT hr  = CoInitializeEx(0, COINIT_MULTITHREADED);
        NW_AUD_WIN_CHECK(hr, NW_AUD_ERROR_INITIALIZATION_FAILED);
        NW_AUD_WIN_CHECK(XAudio2Create(&ctx, 0, XAUDIO2_DEFAULT_PROCESSOR),NW_AUD_ERROR_INITIALIZATION_FAILED);
        NW_AUD_WIN_CHECK(ctx->CreateMasteringVoice(&defaultMasterVoice), NW_AUD_ERROR_INITIALIZATION_FAILED);
        return 0;
    }

    void* getCtx() {
        return (void*)ctx;
    }

    void* getDefMaster() {
        return defaultMasterVoice;
    }

    int setDefMasterVol(float vol) {
        auto s = CAST(IXAudio2MasteringVoice*, defaultMasterVoice);
        NW_AUD_WIN_CHECK(s->SetVolume(vol), NW_AUD_ERROR);
        return 0; 
    }

    void destroyCtx() {
        defaultMasterVoice->DestroyVoice();
        ctx->StopEngine();
        ctx->Release();
        defaultMasterVoice = 0;
        ctx = 0;
    };

/*
Courtesy of http://www.topherlee.com/software/pcm-tut-wavformat.html
Other formats: CPhttps://learn.microsoft.com/en-us/windows/win32/api/xaudio2/nf-xaudio2-ixaudio2-createsourcevoice

Positions   Sample Value         Description
1 - 4       "RIFF"               Marks the file as a riff file. Characters are each 1. byte long.
5 - 8       File size (integer)  Size of the overall file - 8 bytes, in bytes (32-bit integer). Typically, you'd fill this in after creation.
9 -12       "WAVE"               File Type Header. For our purposes, it always equals "WAVE".
13-16       "fmt "               Format chunk marker. Includes trailing null
17-20       16                   Length of format data as listed above
21-22       1                    Type of format (1 is PCM) - 2 byte integer
23-24       2                    Number of Channels - 2 byte integer
25-28       44100                Sample Rate - 32 bit integer. Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz.
29-32       176400               (Sample Rate * BitsPerSample * Channels) / 8.
33-34       4                    (BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo
35-36       16                   Bits per sample
37-40       "data"               "data" chunk header. Marks the beginning of the data section.
41-44       File size (data)     Size of the data section, i.e. file size - 44 bytes header.
*/
    #define CMPN_ERR(v,vv,e,f) \
        if (v!=vv){f.close(); return e;}
    int loadWav(const char* path, DataBuffer outData) {
        WaveData& data = *CAST(WaveData*, outData);
        std::fstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
            return NW_AUD_ERROR_FILE_OPEN;
        FOURCC chid   = 0;
        DWORD  chsize = 0;
        size_t totalSize = 0;
        file.read((char*)&chid, sizeof(FOURCC));
        chid = SWAP_ENDIAN_32(chid);
        CMPN_ERR(chid, 'RIFF', NW_AUD_CORRUPTED_WAVE,file);
        file.read((char*)&chsize, sizeof(DWORD));
        totalSize = chsize;
        //Read 'Wave'
        file.read((char*)&chid, sizeof(FOURCC));
        chid = SWAP_ENDIAN_32(chid);
        CMPN_ERR(chid, 'WAVE', NW_AUD_CORRUPTED_WAVE,file);
        //Read 'fmt '
        file.read((char*)&chid, sizeof(FOURCC));
        chid = SWAP_ENDIAN_32(chid);
        CMPN_ERR(chid, 'fmt ', NW_AUD_CORRUPTED_WAVE,file);
        file.read((char*)&chsize, sizeof(DWORD));
        //CMPN_ERR(chsize, 16, NW_AUD_NOT_PCM, file);
        //Fill structure
        file.read((char*)&data.fmt, chsize);
        CMPN_ERR(data.fmt.wFormatTag, 1, NW_AUD_NOT_PCM, file);
        //Ignore chucks till 'data'
        chsize = 0;
        do {
            file.seekg(chsize, std::ios::cur);
            file.read((char*)&chid, sizeof(FOURCC));
            file.read((char*)&chsize, sizeof(DWORD));
            chid = SWAP_ENDIAN_32(chid);
            if (file.eof()) {
                file.close();
                return NW_AUD_CORRUPTED_WAVE;
            }
        }
        while (chid != 'data');
        data.data.resize(chsize);
        file.read((char*)data.data.data(), chsize);
        return 0;
    };
};
