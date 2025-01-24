#include "gl_context.h"
#include <Windows.h>
#include <wingdi.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_SWAP_COPY_ARB                 0x2029

//Extensions strings for getprocaddr
#define WGL_CHOOSE_PIXEL_FORMAT_ARB_STR "wglChoosePixelFormatARB"
#define WGL_CREATE_CONTEXT_ATTRIBS_ARB  "wglCreateContextAttribsARB"
#define WGL_SWAP_INTERVAL_EXT_STR       "wglSwapIntervalEXT"
#define WGL_GET_SWAP_INTERVAL_EXT_STR   "wglGetSwapIntervalEXT"

typedef BOOL   (WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC)     (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC  (WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)  (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL   (WINAPI* PFNWGLSWAPINTERVALEXTPROC)		    (int interval);
typedef int    (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC)       (void);

PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr; //To disable compatibility and select certain OpenGL version only
PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARB    = nullptr;

PFNWGLSWAPINTERVALEXTPROC		  wglSwapIntervalEXT		 = nullptr;
PFNWGLGETSWAPINTERVALEXTPROC      wglGetSwapIntervalEXT      = nullptr;
namespace NWin {


GlContext* GlContext::_current = nullptr;

GlContext* GlContext::getCurrent() {
	return GlContext::_current;
}

Window* GlContext::getWindow() {
	return _attachedWindow;
}

bool GlContext::isCurrent() {
	return GlContext::_current == this;
}

struct PixelFormat {
	int choice = 0; //Holds the value returned by Choose
	PIXELFORMATDESCRIPTOR pFormat;
};

static void fillpixelFormat(PIXELFORMATDESCRIPTOR& pFormat) {
	pFormat = {};
	//Fixed values by the doc, maybe for future multiple formatdescriptor types https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-pixelformatdescriptor
	pFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pFormat.nVersion = 1;
	//Other values--------------------
	pFormat.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;  //PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI;
	pFormat.iPixelType = PFD_TYPE_RGBA;
	pFormat.cColorBits = 32;
	pFormat.cAlphaBits = 8;
	pFormat.cDepthBits = 24;
}


NWIN_GL_STATUS GlContext::create(Window* w, const OpenGLInfo& info) {
#define FAKE_WIN_MAX_VER 3
	PixelFormat	  pf; //TODO::Move this elsewhere
	NWin::Window* window; //Fake window

	if (info.maxVersion >= FAKE_WIN_MAX_VER) {
	//Create fake window to init OpenGL
		NWin::WindowCrtInfo c{};
		c.metrics.pos = { 0,0 };
		c.description = "Fake";
		c.metrics.size = { 0,0 };
		window = NWin::Window::stCreateWindow(c);
		window->disableTitleBar();
	}
	else {
		window = w;
	}
	
	_attachedWindow = window;
	fillpixelFormat(pf.pFormat);
	
	
	WIN_CHECK21(pf.choice = ChoosePixelFormat((HDC)window->_getDcHandle(), &pf.pFormat), return NWIN_GL_STATUS::PIXEL_FORMAT_CHOICE_FAILURE);
	WIN_CHECK21(SetPixelFormat((HDC)window->_getDcHandle(),pf.choice,&pf.pFormat),	     return NWIN_GL_STATUS::SET_PIXEL_FORMAT_FAILURE);


	WIN_CHECK21(_contextHandle = wglCreateContext((HDC)window->_getDcHandle()), return NWIN_GL_STATUS::CONTEXT_CREATION_FAILURE);

	makeCurrent();

	//Now we have a context, we have to load opengl extension functions---------------------------------------------------
	WIN_CHECK(wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress(WGL_CREATE_CONTEXT_ATTRIBS_ARB));
	WIN_CHECK(wglChoosePixelFormatARB	 = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress(WGL_CHOOSE_PIXEL_FORMAT_ARB_STR));
	WIN_CHECK(wglGetSwapIntervalEXT		 = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress(WGL_GET_SWAP_INTERVAL_EXT_STR));
	WIN_CHECK(wglSwapIntervalEXT         = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress(WGL_SWAP_INTERVAL_EXT_STR));
	//---------------------------------------------------------------------------------------


	if (info.maxVersion < FAKE_WIN_MAX_VER) 
		return NWIN_GL_STATUS::NONE;

	//Next steps are for intialisation of modern opengl and destruction of fake window
	
	int attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, info.maxVersion,
	WGL_CONTEXT_MINOR_VERSION_ARB, info.minVersion, 
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0x2094, 0x0002, 
	0 // ,WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB TODO::Define these, it is to disable compatibility
	};

	int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		0
	};

	window = w; //Get back true window
	if (info.maxVersion >= FAKE_WIN_MAX_VER) {
		Window::stDestroyWindow(_attachedWindow); //Destroy temp window
	}
	_attachedWindow = window;
	HGLRC newC;
	UINT NUM;

	WIN_CHECK( wglChoosePixelFormatARB((HDC)_attachedWindow->_getDcHandle(), pixelAttribs, 0, 1, &pf.choice, &NUM) );
	WIN_CHECK21(DescribePixelFormat((HDC)w->_getDcHandle(),pf.choice, sizeof(pf.pFormat), &pf.pFormat), return NWIN_GL_STATUS::PIXEL_FORMAT_CHOICE_FAILURE);
	WIN_CHECK21(SetPixelFormat((HDC)w->_getDcHandle(), pf.choice, &pf.pFormat), return NWIN_GL_STATUS::SET_PIXEL_FORMAT_FAILURE);

	WIN_CHECK(newC = wglCreateContextAttribsARB((HDC)_attachedWindow->_getDcHandle(), 0, attribs));
	makeCurrent(1);
	WIN_CHECK(wglDeleteContext((HGLRC)_contextHandle));
	_contextHandle = newC;

	return NWIN_GL_STATUS::NONE;
}

NWIN_GL_STATUS GlContext::makeCurrent(bool noContext) {
	if (noContext) { 
		wglMakeCurrent(NULL,NULL); GlContext::_current = nullptr; 
		return NWIN_GL_STATUS::NONE;
	}
	WIN_CHECK21(wglMakeCurrent((HDC)_attachedWindow->_getDcHandle(), (HGLRC)_contextHandle), \
		return NWIN_GL_STATUS::CONTEXT_MAKE_CURRENT_FAILURE
	);
	GlContext::_current = this;
	return NWIN_GL_STATUS::NONE;
}

void GlContext::setCurCtxVSync(int interval) {
	WIN_CHECK(wglSwapIntervalEXT(interval));
}

}
