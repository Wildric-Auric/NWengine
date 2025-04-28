#pragma once
#include <vector>
#include "Globals.h"

class Wave {
    protected:
    float _freq;
    float _ampl;
    float _off;
    public:

    void SetFreq(const float);
    float GetFreq();
    void SetAmpl(const float);
    float GetAmpl();
    void SetOff(const float);
    float GetOff();
    Wave(const float freq = 1.0, const float ampl = 0.0, const float off = 0.0);
    virtual float Evaluate(const float);
};

class SineWave : public Wave {
    public:
    SineWave(const float freq = 1.0, const float ampl=1.0, const float off = 0.0); //                                                                     
    float Evaluate(const float) override;
};

class SquareWave : public Wave {
    public:
    SquareWave(const float freq = 1.0, const float ampl=1.0, const float off = 0.0); // : Wave(freq, ampl, off);                                                                    
    float Evaluate(const float) override;
};

class TriangleWave : public Wave {
    public:
    TriangleWave(const float freq = 1.0, const float ampl=1.0, const float off = 0.0); // : Wave(freq, ampl, off);                                                                    
    float Evaluate(const float) override;
};

class SawTooth : public Wave {
    public:
    SawTooth(const float freq = 1.0, const float ampl=1.0, const float off = 0.0); // : Wave(freq, ampl, off);                                                                    
    float Evaluate(const float) override;
};

class WaveComposer : public Wave {
    std::vector<Wave*> _data; 

    Wave* Add();
    Wave* Add(Wave* const wave);
    void Add(Wave** const, int num);
    void Remove(int index);
    void Remove();
    float Evaluate(const float t);
};

extern constexpr uint32 NWLcg(uint32 seed);

extern constexpr uint64 NWLcg(uint64 seed);

extern constexpr uint32 NWLcgI32(uint32 seed);

extern constexpr uint64 NWLcgI64(uint64 seed);

template <int N, int seed>
struct RandomArray {
    uint32 data[N]{};
    RandomArray() {
        uint32 last = seed;
        for (int i = 0; i < N; ++i) {
            data[i] = NWLcgI32(last);
            last = data[i];
        }
    }
    constexpr uint32 Get(int i) {return data[i];}
};

template <int N, int M, int seed>
struct RandomMatrix {
    RandomArray<N*M, seed> data = RandomArray<N*M,seed>();
    uint32 Get(int i, int j) {return data.Get(i*N+j);}
};

class NWRandom {
    public:
    uint32 seed = 1;
    uint32 tmp = 1;
    uint32 (*randomFunction)(uint32);
    NWRandom(uint32 seed, uint32 (*)(uint32) = NWLcgI32);
    void   SetRandomFunction(uint32 (*)(uint32));
    void   SetSeed(uint32);
    uint32 GetNext();
};

class ValueNoise : Wave {
    NWRandom _rand = NWRandom(10);
    ValueNoise(const float freq = 1.0, const float ampl=1.0, const float off = 0.0); 
    float Evaluate(const float) override;
};
