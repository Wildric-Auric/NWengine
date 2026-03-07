
#include "Context.h"
#include "Globals.h"
#include "wyn/wyn.h"
#include "gl_context.h"

#include "GL/glew.h"

void* Context::window = nullptr;
bool  Context::_shouldLoop = 1;

int Context::WINDOW_WIDTH  = 1080;
int Context::WINDOW_HEIGHT = 720;

int Context::_vSync = 0;

OpenGLInfo Context::_glInfo;

void sizeCallBack(void* handle, NWin::Vec2 size) {
	Context::WINDOW_WIDTH  = size.x;
	Context::WINDOW_HEIGHT = size.y;
}

void sizeCallback2(wyndow* w, wyn_vec2* size, void* usr_data) {
    Context::WINDOW_WIDTH  = size->x;
    Context::WINDOW_HEIGHT = size->y;
}

void Context::GetWinDrawAreaSize(fVec2* v) {
#ifdef PLTFRM_LINUX
    wyn_vec2 s;
    wyndow* w = (wyndow*)Context::window;
    wyn_get_metrics(w, &s,0);
    v->x = s.x;
    v->y = s.y;
#else
	NWin::Vec2	  s;
	NWin::Window* win = (NWin::Window*)Context::window;
	win->getDrawAreaSize(s);
	v->x = s.x;
	v->y = s.y;
#endif
}

void Context::GetWinSize(fVec2* v) {
	v->x = WINDOW_WIDTH;
	v->y = WINDOW_HEIGHT;
}

void Context::SetViewPort(int x, int y, int sizeX, int sizeY) { NW_GL_CALL(glViewport(x, y, sizeX, sizeY)); }

void Context::SetFullscreen(bool state) {
#ifdef PLTFRM_LINUX
    //TODO::
#else
	NWin::Window* win = ((NWin::Window*)Context::window);
	if(state) {
		win->enableFullscreen();
		return;
	}
	win->disableFullscreen();
#endif
}

#ifdef PLTFRM_LINUX
static wyn_glctx context;
#else
static NWin::GlContext context; // TODO::REFACTOR this!!!!
#endif
void*  Context::InitContext(int scrWidth, int scrHeight) { 
#ifdef PLTFRM_LINUX 
     wyndow* w = new wyndow;
     wyn_crt_info       c{};
     wyn_glctx_crt_info gli{};
     c.desc      = (char*)"NWengine";
     c.rect.pos  = {100, 100};
     c.rect.size = {scrWidth, scrHeight};
     wyn_create(w, &c); 
     wyn_rz_cbk_reg(w, sizeCallback2, 0);
     WINDOW_WIDTH  = scrWidth;     
     WINDOW_HEIGHT = scrHeight;
     Context::window = w;
     gli.compatibilityProfile = 0;
     gli.major                = _glInfo.maxVersion;
     gli.minor                = _glInfo.minVersion;
     gli.compatibilityProfile = !_glInfo.disableCompatibility;
     wyn_glctx_create(w, &context, &gli);
     wyn_glctx_make_current(w, &context);
#else
	 NWin::Window*		 w;
	 NWin::WindowCrtInfo c{};
	 c.metrics.pos	= {100, 100};
	 c.description	= "NWengine";
	 c.metrics.size = {scrWidth, scrHeight};
	 c.style		= (NWin::Word)NWin::WindowStyle::Default;
	 c.exStyle		= (NWin::Word)NWin::WindowExStyle::Default;
	 w				= NWin::Window::stCreateWindow(c);

	 NWin::Vec2 s;
	 w->getDrawAreaSize(s);
	 WINDOW_WIDTH  = s.x;
	 WINDOW_HEIGHT = s.y;

	 w->setResizeCallback(sizeCallBack);
	 w->dwmDarkModeFrame(1);
	 w->dwmDontRoundCorners(0);
	 // Context------------------
	 NWin::OpenGLInfo gli;
	 gli.minVersion			  = _glInfo.minVersion;
	 gli.maxVersion			  = _glInfo.maxVersion;
	 gli.disableCompatibility = _glInfo.disableCompatibility;
	 context.create(w, gli);
	 context.makeCurrent();

	 window = w;
#endif

	 if(glewInit() != GLEW_OK) {
		 NW_LOG_ERROR("Failed to init GLEW");
		 return nullptr;
	 }
	 NW_GL_CALL(glViewport(0, 0, scrWidth, scrHeight));
	 return window;
}

bool Context::ShouldClose() { 
#ifdef PLTFRM_LINUX 
    wyndow* w = (wyndow*)Context::window;
    return !_shouldLoop || w->state.should_close;
#else
    return !_shouldLoop || !((NWin::Window*)(Context::window))->shouldLoop(); 
#endif
}

void Context::DeferEndLoop() { _shouldLoop = 0;}

void Context::Update() {
#ifdef PLTFRM_LINUX 
    wyndow* w = (wyndow*)Context::window;
    wyn_update(w);
    wyn_swap(w);
#else
	NWin::Window* w = ((NWin::Window*)(Context::window));
	w->_getKeyboard().update();
	w->swapBuffers();
	w->update();
#endif
}

void Context::EnableVSync(int status) {
	if(Context::_vSync == status)
		return;
	_vSync = status;
#ifdef PLTFRM_LINUX
    wyndow* w = (wyndow*)Context::window;
    wyn_set_vsync(w, _vSync);
#else
	NWin::GlContext::setCurCtxVSync(_vSync);
#endif
}

void Context::EnableBlend(bool status) {
	if(status) {
		NW_GL_CALL(glEnable(GL_BLEND));
		NW_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		return;
	}

	NW_GL_CALL(glDisable(GL_BLEND));
}

void Context::SetBlendFunc(int srcrgb, int dstrgb, int srcalpha, int dstalpha) {
	NW_GL_CALL(glBlendFuncSeparate(srcrgb, dstrgb, srcalpha, dstalpha));
}

void Context::EnableDepthTest(bool status) {
	if(status) {
		NW_GL_CALL(glEnable(GL_DEPTH_TEST));
		return;
	}
	NW_GL_CALL(glDisable(GL_DEPTH_TEST));
}

void Context::EnableStencilTest(bool status) {
	if(status) {
		NW_GL_CALL(glEnable(GL_STENCIL_TEST));
		NW_GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
		SetStencilFunc(NWStencilBehaviour::NW_ALWAYS);
		SetStencilMask(0);

		return;
	}
	NW_GL_CALL(glDisable(GL_STENCIL_TEST));
}

void Context::SetStencilFunc(NWStencilBehaviour b) { NW_GL_CALL(glStencilFunc(b, 1, 0xFF)); }

void Context::SetStencilMask(bool val) { NW_GL_CALL(glStencilMask(val * 0xFF)); }

void Context::EnableWireframe(bool status) {
	if(status) {
		NW_GL_CALL(glDisable(GL_BLEND));
		NW_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		return;
	}
	NW_GL_CALL(glEnable(GL_BLEND));
	NW_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	NW_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void Context::Clear(float r, float g, float b, float a) { /// RGBA
	NW_GL_CALL(glClearColor(r, g, b, a));
	NW_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Context::Clear(const fVec4& col) {
	NW_GL_CALL(glClearColor(col.r, col.g, col.b, col.a));
	NW_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Context::ClearStencilBuff() { NW_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT)); }

void Context::ClearColorBuff(const fVec4& col) {
	NW_GL_CALL(glClearColor(col.r, col.g, col.b, col.a));
	NW_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Context::ClearDepthBuff() { NW_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT)); }

void Context::SetStencilWrite(bool val) {
	if(!val) {
		NW_GL_CALL(glStencilMask(0x00));
		return;
	}
	NW_GL_CALL(glStencilMask(0xFF));
}

void Context::NWMemoryBarrier(int b) { NW_GL_CALL(glMemoryBarrier(b)); }

void Context::SetTitle(const char* title) { 
#ifdef PLTFRM_LINUX
    wyndow* w = (wyndow*)Context::window;
    wyn_set_title(w, title);
#else
    NWin::Window* w = ((NWin::Window*)Context::window);
    w->setTitle(title);
#endif
}

void Context::Destroy() {
#ifdef PLTFRM_LINUX 
    wyndow* w = (wyndow*)Context::window;
    wyn_glctx_destroy(&context);
    wyn_destroy(w);
    delete w;
    Context::window = 0;
#else
	NWin::Window::stDestroyWindow((NWin::Window*)(Context::window));
	context.makeCurrent(1);
#endif
}

namespace GPUCap {
int QueryMaxTexture() {
	int maxUnits = 16;
	NW_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits));
	return Max(1,maxUnits - 1);
}

bool QueryComputeShaderCap(ComputeShaderCapabilities* cap) {
	if(Context::_glInfo.maxVersion < 4 || Context::_glInfo.minVersion < 3) {
		cap->maxInvoc = 0;
		return 0;
	}
	for(int i = 0; i < 3; ++i) {
		NW_GL_CALL(glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &cap->workGroupNum.x + i));
		NW_GL_CALL(glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &cap->localSize.x + i));
	}
	NW_GL_CALL(glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &cap->maxInvoc));
	return 1;
}
}
