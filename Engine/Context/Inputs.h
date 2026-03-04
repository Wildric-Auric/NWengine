/**
 * @file Inputs.h
 * @brief This file contains the declaration of the Inputs class and related input constants.
 */

#pragma once
#include "Globals.h"

typedef int keyN;

enum NWInputKey_ {
    NWInputKey_RMouse     = 1000,
    NWInputKey_LMouse     = 1001,
    NWInputKey_LShift     = 1003,
    NWInputKey_RShift     = 1004,
    NWInputKey_Up         = 1005,
    NWInputKey_Down       = 1006,
    NWInputKey_Right      = 1007,
    NWInputKey_Left       = 1008,
    NWInputKey_Space      = 1009,
    NWInputKey_Ret        = 1010,
};

enum InputKeyEvent {
	KeyPressed,
	OnKeyRelease,
	OnKeyPress,
};

class Inputs {
  public:
	static fVec2 _mousePos;
	static bool left;
    static bool right;
	static bool up;
	static bool down;
	static bool usingJoystick;
	static void Process(void* window);
	static bool GetInputKey(keyN key, InputKeyEvent mode);
	static bool GetInputKeyPressed(keyN);
	static bool GetInputOnKeyRelease(keyN);
	static bool GetInputOnKeyPress(keyN);
	static bool GetInputMouse(keyN key, InputKeyEvent mode);
	static fVec2 GetMousePosition();
	static float joystickAxis[6];
};
