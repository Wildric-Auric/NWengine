#pragma once 
#include "Context.h"
#include "RessourcesLoader.h"

class FrameBuffer {
private:
	uint32 framebuffer;
	uint32 renderbuffer;
public: 
	Texture RenderedImage;
	FrameBuffer(uint16 sizeX = Context::NATIVE_WIDTH, uint16 sizeY = Context::NATIVE_HEIGHT);
	void Bind();
	void Unbind();
};