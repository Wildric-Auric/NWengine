#pragma once
#include "Maths.h"

#define INPUT_LEFT_X 0
#define INPUT_LEFT_Y 1
#define INPUT_RIGHT_X 3
#define INPUT_RIGHT_Y 4

typedef int Input_Number;
typedef int Input_Mode;

class Inputs {
public:
	static double mousePosX,
				  mousePosY;
	static bool left,
				right,
				up,
				down,
				d,
				r,
				s,
				t,
				ctrl,
				left_ctrl,
				right_ctrl,
				left_click,
				n_0,
				n_1,
				n_2,
				n_3,
				n_4,
				enter,
				f2,
				space,
				usingJoystick;

	static void	 Process(void* window);
	static bool	 GetInputKey(Input_Number key, Input_Mode mode);
	static bool  GetInputMouse(Input_Number key, Input_Mode mode);
	static float joystickAxis[6];
};

