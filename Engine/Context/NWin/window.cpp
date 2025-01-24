#include "window.h"

#include <string>
#include <vector>

#include<Windows.h>
#include<winuser.h>
#include <winres.h>
#include <dwmapi.h>

typedef LRESULT  (CALLBACK* win_proc_ptr)(HWND, UINT, WPARAM, LPARAM);

#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#define DWMWA_WINDOW_CORNER_PREFERENCE 33

typedef enum {
	DWMWCP_DEFAULT = 0,
	DWMWCP_DONOTROUND = 1,
	DWMWCP_ROUND = 2,
	DWMWCP_ROUNDSMALL = 3
} DWM_WINDOW_CORNER_PREFERENCE;


namespace NWin {


Rect defaultWindowMetrics = { 100,100,480,360 };


uint64_t Window::_incID = 1;
std::unordered_map<winHandle, Window> Window::_windowsMap;


void getWinRect(const Rect& r, RECT& outRect) {
	outRect.left   = r.pos.x;
	outRect.top    = r.pos.y;
	outRect.bottom = r.pos.y + r.size.y;
	outRect.right  = r.pos.x + r.size.x;
}

LRESULT CALLBACK defaultWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	KeyEvent event;
	Window* win = Window::stGetWindow((winHandle)hwnd);
	switch (uMsg)
	{
		//Inputs------------
		case WM_KEYDOWN:
			event.key		= (Key)wParam;
			event.eventType = NWIN_KeyPressed;
			win->_getKeyboard().record(event);
			break;

		case WM_KEYUP:
			event.key = (Key)wParam;
			event.eventType = NWIN_KeyReleased;
			win->_getKeyboard().record(event);
			break;

		case WM_LBUTTONDOWN:
			event.key = NWIN_KEY_LBUTTON;
			event.eventType = NWIN_KeyPressed;
			win->_getKeyboard().record(event);
			break;

		case WM_LBUTTONUP:
			event.key = NWIN_KEY_LBUTTON;
			event.eventType = NWIN_KeyReleased;
			win->_getKeyboard().record(event);
			break;

		case WM_RBUTTONDOWN:
			event.key = NWIN_KEY_RBUTTON;
			event.eventType = NWIN_KeyPressed;
			win->_getKeyboard().record(event);
			break;

		case WM_RBUTTONUP:
			event.key = NWIN_KEY_RBUTTON;
			event.eventType = NWIN_KeyReleased;
			win->_getKeyboard().record(event);
			break;
		//-------------------

		case WM_CREATE: 
		{
			RECT rcClient;
			GetWindowRect(hwnd, &rcClient);  
			SetWindowPos(hwnd, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom -rcClient.top, SWP_FRAMECHANGED);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			if (win != nullptr)
				ReleaseDC((HWND)win->_getHandle(), hdc);
			return 0;
		}
		case WM_SIZE: {
			if (win == nullptr) return 0;
			NWIN_CALL_CALL_BACK(win->resizeCallback, (winHandle)hwnd, {LOWORD(lParam), HIWORD(lParam)});
			NWIN_CALL_CALL_BACK(win->drawCallback, (winHandle)hwnd);
			return 0;
		};
		case WM_DESTROY: {
			Window::stShouldNotUpdate(hwnd);
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool Window::shouldLoop() {
	return _shouldLoop;
}

int Window::update() {
	//TODO::Add update rate
	while (PeekMessage((LPMSG)_msgBuff, 0, 0, 0, PM_NOREMOVE)) {
		_shouldLoop = GetMessage((LPMSG)_msgBuff, 0, 0, 0);
		TranslateMessage((LPMSG)_msgBuff);
		DispatchMessage((LPMSG)_msgBuff);
	}
	NWIN_CALL_CALL_BACK(drawCallback, (winHandle)_handle)
	return 1;
}

int Window::swapBuffers() {
	SwapBuffers((HDC)_dcHandle);
	return 1;
}

void Window::setResizeCallback(procResizeCallback p) {
	resizeCallback = p;
}

void Window::setDrawCallback(procDrawCallback p) {
	drawCallback = p;
}

void Window::setGdiDrawCallback(procDrawCallback p) {
	gdiDrawCallback = p;
}

void Window::setWndProcCallback(void* p) {
   SetWindowLongPtr((HWND)_handle, GWLP_WNDPROC, (LONG_PTR)p);
}


Window* Window::_stCreateRawWindow(WindowCrtInfo& crtInfo) {
	return nullptr;
}

Window* Window::stCreateWindow(WindowCrtInfo& crtInfo) {
	HINSTANCE moduleInstance;
	LPCTSTR nameID;
	WNDCLASS wc{};
	RECT  winRect{};
	HANDLE h;

	h = NULL;

	moduleInstance = GetModuleHandle(NULL);
	std::string tempN = (std::string("_NWin_") + std::to_string(GET_NEW_ID));
	nameID = tempN.c_str();
	wc.lpszClassName = nameID;
	wc.lpfnWndProc = crtInfo.customWindowProcPtr != nullptr ? (win_proc_ptr)(crtInfo.customWindowProcPtr) : &defaultWinProc;
	wc.hInstance	 = moduleInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor	     = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&wc);

	getWinRect(crtInfo.metrics, winRect);

    if (crtInfo.drawAreaMetrics) {
		AdjustWindowRect(&winRect, crtInfo.style, false);
	}
	
	WIN_CHECK(h = CreateWindowExA(crtInfo.exStyle,
		nameID, crtInfo.description,
		crtInfo.style, winRect.left, winRect.top,
		winRect.right - winRect.left, winRect.bottom - winRect.top, 0, 0, moduleInstance, 0));
	if (!h) return nullptr;
	//Register and initialize members
	Window::_windowsMap.emplace(h, Window());
	Window& win = Window::_windowsMap[h];
	win._instance =  moduleInstance;
	win._handle = h;
	win._msgBuff = new MSG;
	win._id = _incID;
	win._dcHandle = GetDC((HWND)h);
	win._style    = crtInfo.style;
	WIN_CHECK(win._dcHandle);
	//Set winapi parameterss
	ShowWindow((HWND)h, SW_SHOWDEFAULT); //Returns false if the window isn't visible already; does not return error directly
	//WIN_CHECK(SetLayeredWindowAttributes((HWND)h, RGB(255, 0, 0), 100, LWA_ALPHA)); Only if WS_TRANSPARENT is set

	win._keyboard.create(crtInfo.inputBufferSize);
	return &win;
}

Window* Window::stGetWindow(winHandle handle) {
	auto iter = Window::_windowsMap.find(handle);
	return (iter == Window::_windowsMap.end()) ? nullptr : &iter->second;
}

int Window::destroy() {
	delete _msgBuff;
	_keyboard.destroy();
	return 1;
}

winHandle Window::_getHandle() {
	return _handle;
}

deviceContextHandle Window::_getDcHandle() {
	return _dcHandle;
}

applicationInstance Window::_getInstance() {
	return _instance;
}

Keyboard& Window::_getKeyboard() {
	return _keyboard;
}

void Window::getMousePosition(Vec2& pos) {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient((HWND)_handle, &p);
	pos.x = p.x;
	pos.y = p.y;
}

void Window::getDrawAreaSize(Vec2& size) {
	RECT rec{};
	GetClientRect((HWND)_handle, &rec);
	size.x = rec.right - rec.left;
	size.y = rec.bottom - rec.top;
}


int Window::stDestroyWindow(winHandle handle) {
	auto iter = Window::_windowsMap.find(handle);
	if (iter != Window::_windowsMap.end()) {
		iter->second.destroy();
	}

	return DestroyWindow((HWND)handle);
}

int  Window::stDestroyWindow(Window* window) {
	HWND handle = (HWND)window->_handle;
	window->destroy();
	return DestroyWindow(handle);
}

void Window::stShouldNotUpdate(Window* window) {
		window->_shouldLoop = 0;
}

void Window::stShouldNotUpdate(winHandle handle) {
		auto iter = Window::_windowsMap.find(handle);
		if (iter == _windowsMap.end()) return;
		iter->second._shouldLoop = 0;
}

void Window::stClean(winHandle handle) {
	auto iter = Window::_windowsMap.find(handle);
	if (iter == _windowsMap.end()) return;

	iter->second.destroy();
	Window::_windowsMap.erase(iter);
}

void Window::stClean(Window* window) {
	window->destroy();
	Window::_windowsMap.erase(window->_handle);
}

bool Window::dwmBlur() {
	HRESULT hr = S_OK;
	DWM_BLURBEHIND bb = { 0 };

	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = true;
	bb.hRgnBlur = NULL;

	// Enable blur-behind.
	hr = DwmEnableBlurBehindWindow((HWND)_handle, &bb);
	if (!SUCCEEDED(hr)) {
		display("-----------------\n WARNING: Could not enable DMA blur \n----------------- ");
	}
	return SUCCEEDED(hr);
}

bool Window::dwmDarkModeFrame(bool flag) {
	BOOL val = flag;
	HRESULT res = DwmSetWindowAttribute((HWND)_handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &val, sizeof(val));
	return SUCCEEDED(res);
}

bool Window::dwmDontRoundCorners(bool flag) {
	DWM_WINDOW_CORNER_PREFERENCE val = flag ? DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND 
										    : DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DEFAULT;	
	HRESULT res = DwmSetWindowAttribute((HWND)_handle, DWMWA_WINDOW_CORNER_PREFERENCE, &val, sizeof(val));
	return SUCCEEDED(res);
}


//TODO::Return if failure
void Window::disableTitleBar() {
	DWORD style = GetWindowLong((HWND)_handle, GWL_STYLE);
	style &= ~_style;
	WIN_CHECK(SetWindowLong((HWND)_handle, GWL_STYLE, style));
}

void Window::enableTitleBar() {
	DWORD style = GetWindowLong((HWND)_handle, GWL_STYLE);
	style |= _style;
	WIN_CHECK(SetWindowLong((HWND)_handle, GWL_STYLE, style));
}

void Window::setTitle(const char* newTitle) {
	SetWindowText((HWND)_handle,newTitle);
}
void Window::getTitle(char* title, int buffLen) {
	GetWindowText((HWND)_handle, title, buffLen);
}


static BOOL CALLBACK getMonitorCallback(
	HMONITOR monitorHandle,
	HDC	   hdc,
	LPRECT lpRect,
	LPARAM dwData
)
{
	std::vector<HMONITOR>* temp = ((std::vector<HMONITOR>*)dwData);
	temp->push_back(monitorHandle);
	return 1;
};

void getMonitor(HDC dcHandle, std::vector<HMONITOR>& outVector) {
	//"Success" is when all monitors are are enumerated, if gerMonitorCallback returns 0, 
	//then EnumDisplayMonitors returns 0
	//https://github.com/MicrosoftDocs/feedback/issues/1011
	WIN_CHECK(EnumDisplayMonitors(dcHandle, NULL, &getMonitorCallback, (LPARAM)&outVector));
}

void Window::enableFullscreen() {
	std::vector<HMONITOR> outVec;
	getMonitor((HDC)_dcHandle, outVec);
	if (outVec.size() <= 0) return;
	MONITORINFO mInfo;
	mInfo.cbSize = sizeof(mInfo);
	WIN_CHECK(GetMonitorInfo(outVec[0], &mInfo));
	Vec2 size = { mInfo.rcMonitor.right - mInfo.rcMonitor.left, mInfo.rcMonitor.bottom - mInfo.rcMonitor.top };
	WIN_CHECK(SetWindowPos((HWND)_handle, NULL, 0, 0, size.x, size.y, 0));
	disableTitleBar();
}

void Window::disableFullscreen(Rect& newMetrics) {
	enableTitleBar();
	WIN_CHECK(SetWindowPos((HWND)_handle, NULL, newMetrics.pos.x, newMetrics.pos.y, newMetrics.size.x, newMetrics.size.y, 0));
}

}
