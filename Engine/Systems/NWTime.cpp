#pragma once
#include "NWTime.h"
#include "Globals.h"
#include "timer.h"


double NWTime::_deltaTime    = 0.0;
double NWTime::_FPS          = 15;
int    NWTime::_frameCount   = 0;
NWin::timeMl NWTime::_currentTime  = 0;
NWin::timeMl NWTime::_lastTime     = 0;
NWin::timeMl NWTime::_lastTime2    = 0;
double NWTime::_deltaTimeSum = 0;

static NWin::Timer _timer;

void NWTime::Init() {
	_timer.initialize();
}

void NWTime::Update() {
	_frameCount += 1;
	_deltaTimeSum += NWTime::_deltaTime;
	if (_frameCount == 60) {
		NWTime::_FPS =  60.0 / _deltaTimeSum;
		_deltaTimeSum = 0;
		_frameCount = 0;
	}
	//_currentTime = _timer.getTime();
	_deltaTime   = _timer.getTime() / 1000.0;
	//_lastTime2   = _lastTime;
	//_lastTime    = _currentTime;
};

const double& NWTime:: GetFPS() {
	return _FPS;
}

const double& NWTime::GetDeltaTime() {
	return _deltaTime;
}

void NWTimer::SetTickCallback(void(*cbk)(void*), void* data) {
    _tickData = data;
    _tickCallback = cbk;
}

bool NWTimer::Update(double t) {
    if (_isPaused) return 0;
    double prev = _buff;
    _buff = Min<double>(_buff + t, _maxx);
    _tick  = (prev < _maxx) && (_buff >= _maxx);
    (_tick && _tickCallback) ? _tickCallback(_tickData) : void() ;
    ((prev >= _maxx) && _loop) ? SetToZero() : void(); 
    return _tick;
}

bool NWTimer::Update() {
    return Update(NWTime::GetDeltaTime());
}

bool NWTimer::HasFinished() {
    return GetVal() >= _maxx;
}
