#pragma once

class NWTime {
public:
	static double _deltaTime;
	static double _FPS;
	static int    _frameCount;
	static double _currentTime;
	static double _lastTime;
	static double _deltaTimeSum;
	static void  Update();
	static const double& GetDeltaTime();
	static const double& GetFPS();
}; 