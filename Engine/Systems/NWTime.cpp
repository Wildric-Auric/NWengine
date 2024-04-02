#pragma once
#include "NWTime.h"
#include "Globals.h"
#include "nwin/timer.h"


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
	_deltaTime   = (double)(_lastTime - _lastTime2) / 1000.0;
	_lastTime2   = _lastTime;
	_currentTime = _timer.getTime();
	_lastTime    = _currentTime;
};

const double& NWTime:: GetFPS() {
	return _FPS;
}

const double& NWTime::GetDeltaTime() {
	return _deltaTime;
}