#include "Inputs.h"
#include "Context.h"
#include "GL/glew.h"

#include "keyboard.h"
#include "wyn/wyn.h"
#include "window.h"

bool Inputs::left, Inputs::right, Inputs::up, Inputs::down;
bool Inputs::usingJoystick;

fVec2 Inputs::_mousePos;

float Inputs::joystickAxis[6] = {0.0f};

int MapKey(int key) {
#define mkc(a,b) case (a): {return b;}
#ifdef PLTFRM_LINUX
    if (key >= 'a' && key <= 'z')
        return key - 'a' + 'A';
    if (key >= 'A' && key <= 'Z')
        return key;
    switch (key) {
        mkc(NWInputKey_LMouse,     Wyn_Key_LMouse);
        mkc(NWInputKey_RMouse,     Wyn_Key_RMouse);
        mkc(NWInputKey_LShift,     Wyn_Key_LShift);
        mkc(NWInputKey_RShift,     Wyn_Key_RShift);
        mkc(NWInputKey_Up,         Wyn_Key_UArrow);
        mkc(NWInputKey_Down,       Wyn_Key_DArrow);
        mkc(NWInputKey_Right,      Wyn_Key_RArrow);
        mkc(NWInputKey_Left,       Wyn_Key_LArrow);
        mkc(NWInputKey_Space,      Wyn_Key_Space);
        mkc(NWInputKey_Ret,        Wyn_Key_Ret);
        mkc(NWInputKey_Del,        Wyn_Key_Del);
        mkc(NWInputKey_Esc,        Wyn_Key_Esc);
        default: return key;
    }
#else
    switch(key) {
        mkc(NWInputKey_LMouse,     NWin::NWIN_KEY_LBUTTON);
        mkc(NWInputKey_RMouse,     NWin::NWIN_KEY_RBUTTON);
        mkc(NWInputKey_LShift,     NWin::NWIN_KEY_LSHIFT);
        mkc(NWInputKey_RShift,     NWin::NWIN_KEY_RSHIFT);
        mkc(NWInputKey_Up,         NWin::NWIN_KEY_UP);
        mkc(NWInputKey_Down,       NWin::NWIN_KEY_DOWN);
        mkc(NWInputKey_Right,      NWin::NWIN_KEY_RIGHT);
        mkc(NWInputKey_Left,       NWin::NWIN_KEY_LEFT);
        mkc(NWInputKey_Space,      NWin::NWIN_KEY_SPACE);
        mkc(NWInputKey_Ret,        NWin::NWIN_KEY_RETURN);
        mkc(NWInputKey_Del,        NWin::NWIN_KEY_DELETE);
        mkc(NWInputKey_Esc,        NWin::NWIN_KEY_ESCAPE);
        default: return key;
    }
#endif
#undef mkc
}

bool Inputs::GetInputKeyPressed(keyN key) {
#ifdef PLTFRM_LINUX 
    wyndow* w = (wyndow*)Context::window;
    return wyn_key_pressed(w, MapKey(key));
#else
	NWin::Window* window = (NWin::Window*)Context::window;
	return window->_getKeyboard().isKeyPressed((NWin::Key)key);
#endif
}

bool Inputs::GetInputOnKeyRelease(keyN key) {
#ifdef PLTFRM_LINUX
    wyndow* w = (wyndow*)Context::window;
    return wyn_on_key_release(w, MapKey(key));
#else
	NWin::Window* window = (NWin::Window*)Context::window;
	return window->_getKeyboard().onKeyRelease((NWin::Key)key);
#endif
}

bool Inputs::GetInputOnKeyPress(keyN key) {
#ifdef PLTFRM_LINUX
    wyndow* w = (wyndow*)Context::window;
    return wyn_on_key_press(w, MapKey(key));
#else
	NWin::Window* window = (NWin::Window*)Context::window;
	return window->_getKeyboard().onKeyPress((NWin::Key)key);
#endif
}

bool Inputs::GetInputKey(keyN key, InputKeyEvent mode) {
	if(mode == InputKeyEvent::KeyPressed)
		return GetInputKeyPressed(key);
	if(mode == InputKeyEvent::OnKeyPress)
		return GetInputOnKeyPress(key);
	if(mode == InputKeyEvent::OnKeyRelease)
		return GetInputOnKeyRelease(key);
	return 0;
}

bool Inputs::GetInputMouse(keyN key, InputKeyEvent mode) { return Inputs::GetInputKey(key, mode); }

fVec2 Inputs::GetMousePosition() { return _mousePos; }

void Inputs::Process(void* window0) {
	fVec2	   winSize;
#ifdef PLTFRM_LINUX
    wyndow* w = (wyndow*)window0;
    wyn_vec2 mpos;
    wyn_get_mouse_pos(w, &mpos);
    Inputs::_mousePos.x = mpos.x;
    Inputs::_mousePos.y = mpos.y;
    wyn_vec2 s;
    wyn_get_metrics(w, &s, 0);
    winSize = {(float)s.x, (float)s.y};
#else
	NWin::Vec2 pos;
	window->getMousePosition(pos);
	Context::GetWinDrawAreaSize(&winSize);
	Inputs::_mousePos	= fVec2(pos.x, pos.y);
#endif
	left			   = Inputs::GetInputKeyPressed(NWInputKey_Left);
	right              = Inputs::GetInputKeyPressed(NWInputKey_Right);
	up                 = Inputs::GetInputKeyPressed(NWInputKey_Up);
	down               = Inputs::GetInputKeyPressed(NWInputKey_Down);
	Inputs::_mousePos.x = _mousePos.x - winSize.x * 0.5;
	Inputs::_mousePos.y = -_mousePos.y + winSize.y * 0.5;
}
