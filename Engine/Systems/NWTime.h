#pragma once
#include "timer.h"

/**
 * @brief The NWTime class represents the time-related functionality.
 */
class NWTime {
public:
    static double _deltaTime; /**< The time difference between the current frame and the previous frame. */
    static double _FPS; /**< The frames per second. */
    static int _frameCount; /**< The number of frames rendered. */
    static NWin::timeMl _currentTime; /**< The current time. */
    static NWin::timeMl _lastTime; /**< The time of the previous frame. */
    static NWin::timeMl _lastTime2; /**< The time of the frame before the previous frame. */
    static double _deltaTimeSum; /**< The sum of all delta times. */

    /**
     * @brief Initializes the NWTime class.
     */
    static void Init();

    /**
     * @brief Updates the NWTime class.
     */
    static void Update();

    /**
     * @brief Gets the delta time.
     * @return The delta time.
     */
    static const double& GetDeltaTime();

    /**
     * @brief Gets the frames per second.
     * @return The frames per second.
     */
    static const double& GetFPS();
};


//TODO::Document
class NWTimer {
public:
    double _buff   = 0;
    double _maxx   = 0;
    bool _isPaused = 0;
    bool _tick     = 0;
    bool _loop     = 0;
    void* _tickData = nullptr;
    void (*_tickCallback)(void* data) = nullptr;
    
    inline NWTimer(double m) { _maxx = m;}
    inline NWTimer(double m, bool loop) { _maxx = m; _loop = loop;}
    inline bool   GetPaused() {return _isPaused;}
    inline double GetVal() {return _buff;}
    inline double GetMax() {return _maxx;}
    inline void SetPaused(bool p = 1) {_isPaused = p;}
    inline void SetMax(double m) {_maxx = m;}
    inline void SetToZero() { _buff = 0; _tick = 0;}
    inline void SetToMax()  { _buff = _maxx; _tick = 0;}   
    inline void SetLoop(bool l = 1) { _loop = l;}
    inline bool GetTick() { return _tick;}

    bool HasFinished();
    void SetTickCallback(void(*)(void*), void*);
    bool Update();
    bool Update(double t);
    
};
