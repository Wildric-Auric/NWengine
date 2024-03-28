#pragma once
#include "nwin/timer.h"
class NWTime {
public:
	static double _deltaTime;
	static double _FPS;
	static int    _frameCount;
	static NWin::timeMl _currentTime;
	static NWin::timeMl _lastTime;
	static double _deltaTimeSum;
	static void  Init();
	static void  Update();
	static const double& GetDeltaTime();
	static const double& GetFPS();
}; 