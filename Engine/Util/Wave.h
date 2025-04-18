#pragma once
#include <vector>

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
