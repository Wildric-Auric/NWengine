#pragma once 
#include "Context.h"
#include "Texture.h"
#include "Maths.h"

class FrameBuffer {
private:
	uint32 _framebuffer  = 0;
	uint32 _renderbuffer = 0;
public: 
	Texture textureBuffer;
	FrameBuffer() = default;
	//Note: Setup does not clean Framebuffer, using it more than once results in a memory leak!
	void SetUp(Vector2<int> size);
	void Bind();
	void Unbind();
	void Delete();
};