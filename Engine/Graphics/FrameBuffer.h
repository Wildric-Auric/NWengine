#pragma once 
#include "Context.h"
#include "RessourcesLoader.h"

class FrameBuffer {
private:
	uint32 framebuffer  = 0;
	uint32 renderbuffer = 0;
public: 
	Texture RenderedImage;
	//Note: Setup does not clean Framebuffer, using it more than once results in a memory leak!
	FrameBuffer(uint16 sizeX = Context::NATIVE_WIDTH, uint16 sizeY = Context::NATIVE_HEIGHT);
	FrameBuffer(const TextureData& texFilteringData, const Vector2<int> size);
	void SetUp(const  TextureData& texFilteringData, Vector2<int> size = Vector2<int>(Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT));
	void Bind();
	void Unbind();
	void Delete();
};