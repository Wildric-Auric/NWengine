#include "FrameBuffer.h"
#include "GL/glew.h"
#include "glfw3.h"

FrameBuffer::FrameBuffer(uint16 sizeX, uint16 sizeY) {
	if (Context::window == nullptr) return;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	RenderedImage = Texture(sizeX, sizeY, nullptr, 1, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderedImage.texture, 0);

	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}
void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}