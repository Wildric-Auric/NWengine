#pragma once
#include <chrono>
namespace NWin {
	typedef unsigned long long timeMl;

	class Timer {
	private:
		std::chrono::high_resolution_clock::time_point _beg;
		std::chrono::high_resolution_clock::time_point _current;
	public:
		void   initialize();
		timeMl getFrameBegTime(); //Get time recorded at beginning of the frame
		timeMl getTime();
		void   update();
	};
};