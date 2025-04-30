#include "Wave.h"

static constexpr float dpi	= 2.0 * PI;
static constexpr float idpi = 0.5 / PI;

Wave::Wave(const float freq, const float ampl, const float off) {
	SetFreq(freq);
	SetAmpl(ampl);
	SetOff(off);
}

void Wave::SetFreq(const float v) { _freq = dpi * v; }

void Wave::SetFreqDirect(const float v) { _freq = v; }

float Wave::GetFreq() { return idpi * _freq; }

void Wave::SetAmpl(const float v) { _ampl = v; }

float Wave::GetAmpl() { return _ampl; }

void Wave::SetOff(const float v) { _off = dpi * v; }

float Wave::GetOff() { return idpi * _off; }

float Wave::Evaluate(const float) { return _ampl; }

SineWave::SineWave(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float SineWave::Evaluate(const float t) { return _ampl * sin(_freq * t + _off); }

SquareWave::SquareWave(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float SquareWave::Evaluate(const float t) {
	float period = 1.0 / _freq;
	float ct	 = fmod(t + _off, period) / period;
	return (ct < 0.5 ? 1.0 : -1.0) * _ampl;
}

TriangleWave::TriangleWave(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float TriangleWave::Evaluate(const float t) {
	float period = 1.0 / _freq;
	float ct	 = fmod(t + _off, period) / period;
	return (2.0f * fabs(2.0f * ct - 1.0) - 1.0f) * _ampl;
}

SawTooth::SawTooth(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float SawTooth::Evaluate(const float t) {
	float period = 1.0 / _freq;
	float ct	 = fmod(t + _off, period) / period;
	return (2.0f * ct - 1.0) * _ampl;
}

Wave* WaveComposer::Add(Wave* const wave) {
	_data.push_back(wave);
	return _data.back();
}

void WaveComposer::Add(Wave** const arr, int num) {
	for(int i = 0; i < num; ++i) {
		_data.push_back(*(arr + i));
	}
}

void WaveComposer::Remove(int index) { _data.erase(_data.begin() + index); }
void WaveComposer::Remove() { _data.pop_back(); }

float WaveComposer::Evaluate(const float t) {
	float value = 0.0f;
	for(Wave* w : _data) {
		value += w->Evaluate(t);
	}
	return value;
}

float WaveComposer::Evaluate(const float x, const float y) {
	float value = 0.0f;
	for(Wave* w : _data) {
		value += ((Wave2*)w)->Evaluate(x, y);
	}
	return value;
}

constexpr uint32 NWLcg(uint32 seed) { return seed * 1664525u + 1013904223u; }

constexpr uint64 NWLcg(uint64 seed) { return seed * 1664525u + 1013904223u; };

constexpr uint32 NWSplitMixI32(uint32 x) {
	x += 0x9e3779b9;
	x ^= x >> 16;
	x *= 0x85ebca6b;
	x ^= x >> 13;
	x *= 0xc2b2ae35;
	x ^= x >> 16;
	return x;
}

// https://prng.di.unimi.it/splitmix64.c
constexpr uint64 NWSplitMixI64(uint64 x) {
	uint64 z = x + 0x9e3779b97f4a7c15;
	z		 = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z		 = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

constexpr uint32 NWSplitMix(uint32 x) { return NWSplitMixI32(x); }
constexpr uint64 NWSplitMix(uint64 x) { return NWSplitMixI64(x); }
constexpr uint32 NWLcgI32(uint32 seed) { return NWLcg(seed); }
constexpr uint64 NWLcgI64(uint64 seed) { return NWLcg(seed); }

constexpr uint32 NWFnva_2UI32_To_1UI32(uint32 a, uint32 b) {
	const uint32 FNV_OFFSET_BASIS = 2166136261u;
	const uint32 FNV_PRIME		  = 16777619u;
	uint32		 hash			  = FNV_OFFSET_BASIS;
	// first
	hash ^= (a >> (0 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	hash ^= (a >> (1 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	hash ^= (a >> (2 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	hash ^= (a >> (3 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	// sec
	hash ^= (b >> (0 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	hash ^= (b >> (1 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	hash ^= (b >> (2 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	hash ^= (b >> (3 * 8)) & 0xFF;
	hash *= FNV_PRIME;
	// ret
	return hash;
}

NWRandom::NWRandom(uint32 seed, uint32 (*f)(uint32)) {
	SetRandomFunction(f);
	SetSeed(seed);
}

void NWRandom::SetRandomFunction(uint32 (*f)(uint32)) { randomFunction = f; }

void NWRandom::SetSeed(uint32 s) {
	seed = s;
	tmp	 = s;
}

uint32 NWRandom::GetNext() { return randomFunction(tmp); }

uint32 NWRandom::Get(uint32 x) { return randomFunction(x ^ seed); }

ValueNoise::ValueNoise(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) { _freq = freq; }

float ValueNoise::Evaluate(const float v) {
	uint32 ipt	 = ((v + _off) / _freq);
	double frac	 = fmod(v + _off, _freq) / _freq;
	frac		 = Smoothstep<double>(frac, 0.0, 1.0);
	uint32 a	 = (_rand.Get(ipt));
	uint32 b	 = (_rand.Get(ipt + 1));
	uint32 value = lerp(a, b, frac);
	return Normalize<double>(value, NW_UI32_MAX, _ampl);
}

Wave2::Wave2(const float freq, const float ampl, const float off) : Wave(freq, ampl, off) {}

float Wave2::Evaluate(float x, float y) { return _ampl; }

ValueNoise2::ValueNoise2(const float freq, const float ampl, const float off) : Wave2(freq, ampl, off) { _freq = freq; }

float ValueNoise2::Evaluate(const float i, const float j) {
	auto   f	 = NWFnva_2UI32_To_1UI32;
	uint32 iptX	 = (i + _off) / _freq;
	uint32 iptY	 = (j + _off) / _freq;
	double fracX = fmod(i + _off, _freq) / _freq;
	double fracY = fmod(j + _off2, _freq) / _freq;
	fracX		 = Smoothstep<double>(fracX, 0.0, 1.0);
	fracY		 = Smoothstep<double>(fracY, 0.0, 1.0);

	uint32 bl	= (_rand.Get(f(iptX, iptY)));
	uint32 br	= (_rand.Get(f(iptX + 1, iptY)));
	uint32 tl	= (_rand.Get(f(iptX, iptY + 1)));
	uint32 tr	= (_rand.Get(f(iptX + 1, iptY + 1)));
	uint32 xxb2 = lerp(bl, br, fracX);
	uint32 xxb	= lerp(bl, br, fracX);
	uint32 xxt	= lerp(tl, tr, fracX);
	uint32 ret	= lerp(xxb, xxt, fracY);

	return Normalize<double>(ret, NW_UI32_MAX, _ampl);
};
