#include "GL/glew.h"

#include "FrameBuffer.h"
#include <string>

void FrameBuffer::SetUp(Vector2<int> size) {
	if (Context::window == nullptr) return;
	textureBuffer._size = size;
	textureBuffer._GPUGen(nullptr, TexChannelInfo::NW_RGB);
	textureBuffer.SetEdgesBehaviour(TexEdge::NW_CLAMP);
	textureBuffer.SetMinFilter(TexMinFilter::NW_MIN_LINEAR);
	textureBuffer.SetMaxFilter(TexMaxFilter::NW_LINEAR);

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	//textureBuffer				= Texture(nullptr, size, texFilteringData);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer._glID, 0);

	glGenRenderbuffers(1, &_renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, textureBuffer._size.x, textureBuffer._size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}
void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Delete() {
	glDeleteRenderbuffers(1, &this->_renderbuffer);
	glDeleteFramebuffers(1,  &this->_framebuffer);
	textureBuffer.Clean();
}