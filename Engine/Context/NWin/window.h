#pragma once
#include <cinttypes>
#include <unordered_map>
#include "util.h"
#include "keyboard.h"


namespace NWin {

#define GET_NEW_ID (Window::_incID++)
#define NWIN_CALL_CALL_BACK(func, ...) if ((func) != nullptr) { func(__VA_ARGS__); }

typedef void* winHandle;
typedef void* MsgBuffer;
typedef void* deviceContextHandle;
typedef	void* applicationInstance;

typedef void(*procResizeCallback)(winHandle, Vec2 newSize);
typedef void(*procDrawCallback)(winHandle);


enum class WindowStyle : Word {
	Default = (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L)
};

enum class WindowExStyle {
	Default = 0x00040000L //->WS_EX_APPWINDOW ; ----- 0x00000020L | 0x02000000L //Transparent | Composited
};


extern Rect defaultWindowMetrics;

#ifdef _WINDOWS_ //Windows.h is included
typedef LRESULT  (CALLBACK* win_proc_ptr)(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK defaultWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
struct WindowCrtInfo {
	char* description         = nullptr;
	Rect  metrics             = defaultWindowMetrics;
	bool  drawAreaMetrics     = true;
	void* customWindowProcPtr = nullptr;
	Word  style				  = (Word)WindowStyle::Default;
	Word  exStyle			  = (Word)WindowExStyle::Default;
	int   inputBufferSize     = 256;
};

class Window {
private:
	uint64_t  _id					= 0;
	winHandle _handle				= nullptr;
	deviceContextHandle _dcHandle	= nullptr;
	applicationInstance  _instance  = nullptr;
	MsgBuffer _msgBuff			    = nullptr;
	bool _shouldLoop			    = 1;
	Word _style						= 0; //Mainly to enable and disable title bar

	Keyboard _keyboard;

	static std::unordered_map<winHandle, Window> _windowsMap;
	static uint64_t _incID;

public:
	Window()						 = default;
	Window(Window&& other)			 = default;
	Window(const Window&)		     = delete;
	Window& operator=(const Window&) = delete;

	winHandle			_getHandle();
	deviceContextHandle _getDcHandle();
	applicationInstance _getInstance();
	Keyboard&			_getKeyboard();

	void getDrawAreaSize(Vec2& size);
	void getMousePosition(Vec2& positon); // Get the mouse position according to client area
	int  update();
	int  swapBuffers();
	int	 destroy();
	bool shouldLoop();
	//Styles setters
	void disableTitleBar();
	void enableTitleBar();

	void setTitle(const char* newTitle);
	void getTitle(char* title, int buffLen);

	void enableFullscreen();
	void disableFullscreen(Rect& newMetrics = defaultWindowMetrics);

	//Callback setters
	void setResizeCallback(procResizeCallback);
	void setDrawCallback(procDrawCallback);
	void setGdiDrawCallback(procDrawCallback);
    void setWndProcCallback(void*);
	//DWM
	bool dwmBlur();					     //Not working; it should blur the background though
	bool dwmDarkModeFrame(bool flag);	 //if the flag is set on true, the window uses user's theme for its frame (non client area) color; otherwise, it is white.
	bool dwmDontRoundCorners(bool flag); //if the flag is set on true, window's corners are not rounded, otherwise the system chooses
	

	//Callback function----------
	procResizeCallback resizeCallback  = nullptr;
	procDrawCallback   drawCallback	  = nullptr;
	procDrawCallback   gdiDrawCallback = nullptr;
	//Static---------------------
	static Window*  stGetWindow(winHandle handle);
	static Window*	stCreateWindow(WindowCrtInfo& crtInfo);

	static int		stDestroyWindow(winHandle handle);
	static int      stDestroyWindow(Window* window);

	static void		stShouldNotUpdate(Window*    window);
	static void		stShouldNotUpdate(winHandle handle);

	static void		stClean(winHandle handle);
	static void		stClean(Window* window);

	static Window* _stCreateRawWindow(WindowCrtInfo& crtInfo);
	};
}



