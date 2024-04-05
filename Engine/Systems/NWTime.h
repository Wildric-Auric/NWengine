#pragma once
#include "nwin/timer.h"

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