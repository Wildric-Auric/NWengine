#pragma once
#include "NWTime.h"
#include "Globals.h"
#include <glfw3.h>
double NWTime::_deltaTime    = 0.0;
double NWTime::_FPS          = 15;
int    NWTime::_frameCount   = 0;
double NWTime::_currentTime  = 0;
double NWTime::_lastTime     = 0;
double NWTime::_deltaTimeSum = 0;

void NWTime::Update() {
	_frameCount += 1;
	_deltaTimeSum += NWTime::_deltaTime;
	if (_frameCount == 60) {
		NWTime::_FPS = 60.0 / _deltaTimeSum;
		_deltaTimeSum = 0;
		_frameCount = 0;
	}
	_currentTime = glfwGetTime();
	_deltaTime = _currentTime - _lastTime;
	_lastTime = _currentTime;
};

const double& NWTime:: GetFPS() {
	return _FPS;
}

const double& NWTime::GetDeltaTime() {
	return _deltaTime;
}