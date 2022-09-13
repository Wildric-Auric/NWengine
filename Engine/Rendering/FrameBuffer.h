#pragma once 
#include "Texture.h"
#include "RessourcesLoader.h"
#include <GL/glew.h>
#include <glfw3.h>
#include "Context.h"
class FrameBuffer {
private:
	uint32 framebuffer;
	uint32 renderbuffer;
public: 
	Texture RenderedImage;
    FrameBuffer(uint16 sizeX = Context::NATIVE_WIDTH, uint16 sizeY = Context::NATIVE_HEIGHT) {
		if (Context::window == nullptr) return;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	    RenderedImage = Texture(sizeX, sizeY, nullptr, 1,0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderedImage.texture, 0);

		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void Bind(){
		 glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}
	void Unbind() {
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};