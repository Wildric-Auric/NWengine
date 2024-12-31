
#include "Context.h"
#include "Globals.h"
#include "nwin/gl_context.h"

#include "GL/glew.h"
#include <iostream>

void* Context::window = nullptr;

int Context::WINDOW_WIDTH =  1080;
int Context::WINDOW_HEIGHT = 720;

int Context::NATIVE_WIDTH = 1080;
int Context::NATIVE_HEIGHT = 720;

int Context::vSync	 = 1;

OpenGLInfo  Context::_glInfo;


void sizeCallBack(NWin::winHandle handle, NWin::Vec2 size)
{
		Context::WINDOW_WIDTH = size.x;
		Context::WINDOW_HEIGHT = size.y;
}

void Context::SetViewPort(int x, int y, int sizeX, int sizeY) {
	NW_GL_CALL(glViewport(x, y, sizeX, sizeY));
}

void Context::SetFullscreen(bool state) {
	NWin::Window* win = ((NWin::Window*)Context::window);
	if (state) {
		win->enableFullscreen();
		return;
	}
	win->disableFullscreen();
}

static NWin::GlContext context; //TODO::REFACTOR this!!!!
void* Context::InitContext(int scrWidth, int scrHeight)
{
	NWin::Window* w;
	NWin::WindowCrtInfo c{};
	c.metrics.pos = { 100,100 };
	c.description = "NWengine";
	c.metrics.size = { scrWidth, scrHeight};
	c.style = (NWin::Word)NWin::WindowStyle::Default;
	c.exStyle = (NWin::Word)NWin::WindowExStyle::Default;
	w = NWin::Window::stCreateWindow(c);

	w->setResizeCallback(sizeCallBack);
	w->dwmDarkModeFrame(1);
	w->dwmDontRoundCorners(0);
	//Context------------------
	NWin::OpenGLInfo gli;
	gli.minVersion = _glInfo.minVersion;
	gli.maxVersion = _glInfo.maxVersion;
	gli.disableCompatibility = _glInfo.disableCompatibility;
	context.create(w, gli);
	context.makeCurrent();

	window = w;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW";
		return nullptr;
	}
	NW_GL_CALL(glViewport(0, 0, Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT));

	return window;
}

bool Context::ShouldClose() {
	return !((NWin::Window*)(Context::window))->shouldLoop();
}

void Context::Update() {
		NWin::Window* w = ((NWin::Window*)(Context::window));
		w->_getKeyboard().update();
		w->update();
		w->swapBuffers();
}

void Context::EnableBlend(bool status) {
	if (status) {
		NW_GL_CALL(glEnable(GL_BLEND));
		NW_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		return;
	}

	NW_GL_CALL(glDisable(GL_BLEND));
	
}

void Context::EnableDepthTest(bool status) {
	if (status) {
		NW_GL_CALL(glEnable(GL_DEPTH_TEST));
		return;
	}
	NW_GL_CALL(glDisable(GL_DEPTH_TEST));
}

void Context::EnableWireframe(bool status) {
	if (status) {
		NW_GL_CALL(glDisable(GL_BLEND));
		NW_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		return;
	}
	NW_GL_CALL(glEnable(GL_BLEND));
	NW_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	NW_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void Context::Clear(float r, float g, float b, float a) { ///RGBA
	NW_GL_CALL(glClearColor(r,g,b,a));
	NW_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


void Context::SetTitle(const char* title) {
	((NWin::Window*)Context::window)->setTitle(title);
}

void Context::Destroy() {
	NWin::Window::stDestroyWindow((NWin::Window*)(Context::window));
	context.makeCurrent(1);
}