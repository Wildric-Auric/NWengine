///I'M HOPPING FIND A WAY TO DO SCHEDUALED EVENTS

//#pragma once
#include "Time.h"
#include <algorithm>
#include <tuple>
class Clock {
public:
	double time[15] = {-1.0};
	int lock[15] = { false };


	//std::tuple<void(*func)(int),int,int> invokedList = std::make_tuple(1,2,3);

	//template<typename T1, typename T2, typename T3>
	//void invoke1(T1(*func)(T2), T3 a, int clockIndex, float delay ) {
	//	if time[clockIndex] == 0 {
	//		time[clockIndex] = delay;
	//		lock[clockIndex] = true;
	//		invokedList = std::make_tuple_cat(std::make_typl)
	//	}
	//};

	//template<typename T1, typename T2, typename T3, typename T4, typename T5>
	//void invoke2(T1(*func)(T2, T3), T4 a, T5 b, int clockIndex) {
	//	func(a, b);
	//};

	//template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	//void invoke3(T1(*func)(T2, T3, T4), T5 a, T6 b, T7 c, int clockIndex) {
	//	func(a, b, c);
	//};

	//template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	//void invoke4(T1(*func)(T2, T3, T4, T5), T6 a, T7 b, T8 c, T9 d, int clockIndex) {
	//	func(a, b, c, d);
	//};

	void StopClockEvent(int index) {
		time[index] = 0.0;
	}
	void UpdateClock(){
		for (int i = 0; i < 15; i++) {
			time[i] -= std::max(0.0, time[i] - deltaTime);
		}
	}
};

