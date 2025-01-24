#include "timer.h"
#include <windows.h>
namespace NWin {

    void Timer::initialize() {
        _beg = std::chrono::high_resolution_clock::now();
    }

    timeMl Timer::getFrameBegTime() {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(_current - _beg);
        return (timeMl)duration.count();
    }

    timeMl Timer::getTime() {
        update();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(_current - _beg);
        return (timeMl)duration.count();
    }

    void Timer::update() {
        _current = std::chrono::high_resolution_clock::now();
    }

};