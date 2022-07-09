#pragma once 
#include "Texture.h"
#include "RessourcesLoader.h"
#include <GL/glew.h>
#include <glfw3.h>
#include "Context.h"
class FrameBuffer {
private:
	uint32 framebuffer;
public: 
	Texture RenderedImage;
    FrameBuffer() {
		if (Context::window == nullptr) return;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	    RenderedImage = Texture(Globals::NATIVE_WIDTH, Globals::NATIVE_HEIGHT, nullptr, 1,0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderedImage.texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void Bind(){
		 glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}
	void Unbind() {
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};