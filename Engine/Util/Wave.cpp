#include "Wave.h"
#include "Maths.h"
#include <math.h>

static constexpr float dpi = 2.0*PI;
static constexpr float idpi = 0.5/PI;

Wave::Wave(const float freq, const float ampl, const float off) {
    SetFreq(freq);
    SetAmpl(ampl);
    SetOff(off);
}

void Wave::SetFreq(const float v) {
    _freq = dpi*v;
}

float Wave::GetFreq() {
    return idpi*_freq;
}

void Wave::SetAmpl(const float v) {
    _ampl = v;
}

float Wave::GetAmpl() {
    return _ampl;
}

void Wave::SetOff(const float v) {
    _off = dpi * v;
}

float Wave::GetOff() {
    return idpi * _off;
}

float Wave::Evaluate(const float) {return _ampl;}

SineWave::SineWave(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float SineWave::Evaluate(const float t) {
    return _ampl*sin(_freq*t + _off);
}

SquareWave::SquareWave(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float SquareWave::Evaluate(const float t) {
    float period = 1.0/_freq;
    float ct = fmod(t + _off, period) / period;
    return (ct < 0.5 ? 1.0 : -1.0)*_ampl;
}

TriangleWave::TriangleWave(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float TriangleWave::Evaluate(const float t) {
    float period = 1.0/_freq;
    float ct = fmod(t + _off, period) / period;
    return (2.0f * fabs(2.0f * ct - 1.0) - 1.0f)*_ampl;
}

SawTooth::SawTooth(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float SawTooth::Evaluate(const float t) {
    float period = 1.0/_freq;
    float ct = fmod(t + _off, period) / period;
    return (2.0f * ct - 1.0)*_ampl;
}

Wave* WaveComposer::Add(Wave* const wave) {
    _data.push_back(wave);
    return _data.back();
}

void WaveComposer::Add(Wave** const arr, int num) {
    for (int i = 0; i < num; ++i) {
        _data.push_back(*(arr+i));
    }
}

void WaveComposer::Remove(int index) {
   _data.erase(_data.begin() + index); 
}
void WaveComposer::Remove() {
    _data.pop_back();
}

float WaveComposer::Evaluate(const float t) {
    float value = 0.0f;
    for (Wave* w : _data) {
        value += w->Evaluate(t); 
    }
    return value;
}
