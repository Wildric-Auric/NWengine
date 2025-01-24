#include "Inputs.h"
#include "Context.h"
#include "GL/glew.h"

#include "keyboard.h"
#include "window.h"

#include<iostream>
#include <cstring>

 bool Inputs::left, Inputs::right, Inputs::up, Inputs::down, Inputs::d, Inputs::r, Inputs::s, Inputs::t,
 Inputs::n_1, Inputs::n_2, Inputs::n_3, Inputs::n_0, Inputs::n_4, Inputs::left_click,
 Inputs::space, Inputs::usingJoystick, Inputs::f2, Inputs::enter = 0,
 Inputs::ctrl,
 Inputs::left_ctrl,
 Inputs::right_ctrl;

double Inputs::mousePosX;
double Inputs::mousePosY;

float Inputs::joystickAxis[6] = {0.0f};

bool Inputs::GetInputKey(Input_Number key, Input_Mode mode) {
    NWin::Window* window = (NWin::Window*)Context::window;
	if (mode == NWin::KeyEventEnum::NWIN_KeyPressed) {
		return window->_getKeyboard().isKeyPressed((NWin::Key)key);
	}
	if (mode == NWin::KeyEventEnum::NWIN_KeyReleased) {
		return window->_getKeyboard().onKeyRelease((NWin::Key)key);
	}
}

//Legacy lol
bool Inputs::GetInputMouse(Input_Number key, Input_Mode mode) {
	return Inputs::GetInputKey(key, mode);
}

void Inputs::Process(void* window0) {
    NWin::Window* window = (NWin::Window*)Context::window;
	window = (NWin::Window*)window0;
	NWin::Vec2 pos;
	window->getMousePosition(pos);
	Inputs::mousePosX = pos.x;
	Inputs::mousePosY = pos.y;

	usingJoystick = 0;
	/*const unsigned char* buttons = 0;
		if (usingJoystick) {
			int count;
			const float* local = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
			int buttonCount;
			buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
			memcpy(joystickAxis, local, count * sizeof(float)); //TODO::See if memcpy is the  Best Solution here
			for (int i = 0; i < count; i++) {
				if (abs(joystickAxis[i]) < 0.01) joystickAxis[i] = 0;
			}
	}*/
	NWin::Keyboard& kb = window->_getKeyboard();
	left       = kb.isKeyPressed(NWin::Key::NWIN_KEY_LEFT);
	right      = kb.isKeyPressed(NWin::Key::NWIN_KEY_RIGHT);
	up	       = kb.isKeyPressed(NWin::Key::NWIN_KEY_UP);
	down       = kb.isKeyPressed(NWin::Key::NWIN_KEY_DOWN);
	d          = kb.isKeyPressed((NWin::Key)'D'); //TODO::Add wrapper function to NWin so that conversion is done within it with correct values
	r          = kb.isKeyPressed((NWin::Key)'R');
	s          = kb.isKeyPressed((NWin::Key)'S');
	t		   = kb.isKeyPressed((NWin::Key)'T');
	left_click = kb.isKeyPressed(NWin::Key::NWIN_KEY_LBUTTON);
	n_0        = kb.isKeyPressed((NWin::Key)'0');
	n_1        = kb.isKeyPressed((NWin::Key)'1');
	n_2        = kb.isKeyPressed((NWin::Key)'2');
	n_3        = kb.isKeyPressed((NWin::Key)'3');
	n_4        = kb.isKeyPressed((NWin::Key)'4');
	space	   = kb.isKeyPressed(NWin::Key::NWIN_KEY_SPACE);
	f2		   = kb.isKeyPressed(NWin::Key::NWIN_KEY_F2);
	enter	   = kb.isKeyPressed(NWin::Key::NWIN_KEY_RETURN);
	left_ctrl  = kb.isKeyPressed(NWin::Key::NWIN_KEY_LCONTROL);
	right_ctrl = kb.isKeyPressed(NWin::Key::NWIN_KEY_RCONTROL);
	ctrl	   =  right_ctrl || left_ctrl;

};
