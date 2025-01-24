#pragma once
#include "window.h"



namespace NWin {

typedef void* contextHandle;

struct OpenGLInfo {
	uchar minVersion = 3;
	uchar maxVersion = 3;
	//bool  profileCore = 1;
	bool  disableCompatibility = 1;
};

enum class NWIN_GL_STATUS {
	NONE = 0,
	PIXEL_FORMAT_CHOICE_FAILURE,
	SET_PIXEL_FORMAT_FAILURE,
	CONTEXT_CREATION_FAILURE,
	CONTEXT_MAKE_CURRENT_FAILURE,
	CORE_OPENGL_INIT_FAILURE
};

class GlContext {
private:
	static GlContext* _current;
	
	Window*			_attachedWindow = nullptr;
	contextHandle	_contextHandle  = nullptr;
public:

	bool			isCurrent();
	NWIN_GL_STATUS	create(Window* window, const OpenGLInfo& glInfo);
	NWIN_GL_STATUS  makeCurrent(bool noContext = 0);
	Window*		    getWindow();
	

	static GlContext* getCurrent();
	static void setCurCtxVSync(int interval = 1);
};

};
