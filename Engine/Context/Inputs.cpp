#include "Inputs.h"
#include "Context.h"
#include "GL/glew.h"

#include "keyboard.h"
#include "window.h"

bool Inputs::left, Inputs::right, Inputs::up, Inputs::down, Inputs::d, Inputs::r, Inputs::s, Inputs::t, Inputs::n_1, Inputs::n_2,
	Inputs::n_3, Inputs::n_0, Inputs::n_4, Inputs::left_click, Inputs::space, Inputs::usingJoystick, Inputs::f2,
	Inputs::enter = 0, Inputs::ctrl, Inputs::left_ctrl, Inputs::right_ctrl;

fVec2 Inputs::_mousePos;

float Inputs::joystickAxis[6] = {0.0f};

bool Inputs::GetInputKeyPressed(keyN key) {
	NWin::Window* window = (NWin::Window*)Context::window;
	return window->_getKeyboard().isKeyPressed((NWin::Key)key);
}

bool Inputs::GetInputOnKeyRelease(keyN key) {
	NWin::Window* window = (NWin::Window*)Context::window;
	return window->_getKeyboard().onKeyRelease((NWin::Key)key);
}

bool Inputs::GetInputOnKeyPress(keyN key) {
	NWin::Window* window = (NWin::Window*)Context::window;
	return window->_getKeyboard().onKeyPress((NWin::Key)key);
}

bool Inputs::GetInputKey(keyN key, InputKeyEvent mode) {
	NWin::Window* window = (NWin::Window*)Context::window;
	if(mode == InputKeyEvent::KeyPressed)
		return window->_getKeyboard().isKeyPressed((NWin::Key)key);
	if(mode == InputKeyEvent::OnKeyPress)
		return window->_getKeyboard().onKeyPress((NWin::Key)key);
	if(mode == InputKeyEvent::OnKeyRelease)
		return window->_getKeyboard().onKeyRelease((NWin::Key)key);
	return 0;
}

bool Inputs::GetInputMouse(keyN key, InputKeyEvent mode) { return Inputs::GetInputKey(key, mode); }

fVec2 Inputs::GetMousePosition() { return _mousePos; }

void Inputs::Process(void* window0) {
	NWin::Window* window = (NWin::Window*)Context::window;
	window				 = (NWin::Window*)window0;
	NWin::Vec2 pos;
	fVec2	   winSize;
	window->getMousePosition(pos);
	Context::GetWinDrawAreaSize(&winSize);
	Inputs::_mousePos	= fVec2(pos.x, pos.y);
	Inputs::_mousePos.x = _mousePos.x - winSize.x * 0.5;
	Inputs::_mousePos.y = -_mousePos.y + winSize.y * 0.5;

	NWin::Keyboard& kb = window->_getKeyboard();
	left			   = kb.isKeyPressed(NWin::Key::NWIN_KEY_LEFT);
	right			   = kb.isKeyPressed(NWin::Key::NWIN_KEY_RIGHT);
	up				   = kb.isKeyPressed(NWin::Key::NWIN_KEY_UP);
	down			   = kb.isKeyPressed(NWin::Key::NWIN_KEY_DOWN);
	d				   = kb.isKeyPressed(
		 (NWin::Key)'D'); // TODO::Add wrapper function to NWin so that conversion is done within it with correct values
	r		   = kb.isKeyPressed((NWin::Key)'R');
	s		   = kb.isKeyPressed((NWin::Key)'S');
	t		   = kb.isKeyPressed((NWin::Key)'T');
	left_click = kb.isKeyPressed(NWin::Key::NWIN_KEY_LBUTTON);
	n_0		   = kb.isKeyPressed((NWin::Key)'0');
	n_1		   = kb.isKeyPressed((NWin::Key)'1');
	n_2		   = kb.isKeyPressed((NWin::Key)'2');
	n_3		   = kb.isKeyPressed((NWin::Key)'3');
	n_4		   = kb.isKeyPressed((NWin::Key)'4');
	space	   = kb.isKeyPressed(NWin::Key::NWIN_KEY_SPACE);
	f2		   = kb.isKeyPressed(NWin::Key::NWIN_KEY_F2);
	enter	   = kb.isKeyPressed(NWin::Key::NWIN_KEY_RETURN);
	left_ctrl  = kb.isKeyPressed(NWin::Key::NWIN_KEY_LCONTROL);
	right_ctrl = kb.isKeyPressed(NWin::Key::NWIN_KEY_RCONTROL);
	ctrl	   = right_ctrl || left_ctrl;
};
