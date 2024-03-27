
#include "Context.h"
#include "Globals.h"
#include "GL/glew.h"

#include "nwin/window.h"
#include "nwin/gl_context.h"

#include <iostream>

void* Context::window = nullptr;

int Context::WINDOW_WIDTH =  1080;
int Context::WINDOW_HEIGHT = 720;

int Context::NATIVE_WIDTH = 1080;
int Context::NATIVE_HEIGHT = 720;

int Context::dllFlag = NW_KEEP_DLL_RUNNING;

int Context::vSync	 = 1;

void sizeCallBack(NWin::winHandle handle, NWin::Vec2 size)
{
		Context::WINDOW_WIDTH = size.x;
		Context::WINDOW_HEIGHT = size.y;
}

void Context::SetViewPort(int x, int y, int sizeX, int sizeY) {
	glViewport(x, y, sizeX, sizeY);
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
	NWin::OpenGLInfo glInfo;
	c.metrics.pos = { 0,0 };
	c.description = "NWengine";
	c.metrics.size = { scrWidth, scrHeight};
	c.style = (NWin::Word)NWin::WindowStyle::Default;
	c.exStyle = (NWin::Word)NWin::WindowExStyle::Default;
	w = NWin::Window::stCreateWindow(c);

	w->setResizeCallback(sizeCallBack);

	w->dwmDarkModeFrame(1);
	w->dwmDontRoundCorners(0);
	//Context------------------
	glInfo.minVersion = 3;
	glInfo.maxVersion = 3;
	context.create(w, glInfo);
	context.makeCurrent();

	window = w;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW";
		return nullptr;
	}
	glViewport(0, 0, Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT);

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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return;
	}

	glDisable(GL_BLEND);
	
}

void Context::EnableDepthTest(bool status) {
	if (status) {
		glEnable(GL_DEPTH_TEST);
		return;
	}
	glDisable(GL_DEPTH_TEST);
}

void Context::EnableWireframe(bool status) {
	if (status) {
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		return;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Context::Clear(float r, float g, float b, float a) { ///RGBA
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Context::SetTitle(const char* title) {
	//TODO::Add title change function to NWin
}
void Context::Destroy() {
	NWin::Window::stDestroyWindow((NWin::Window*)(Context::window));
	context.makeCurrent(1);
}