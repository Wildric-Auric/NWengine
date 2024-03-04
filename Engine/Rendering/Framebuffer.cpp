#include "FrameBuffer.h"
#include "GL/glew.h"
#include "glfw3.h"

FrameBuffer::FrameBuffer(uint16 sizeX, uint16 sizeY) {
	TextureData texData;
	texData.genMipMap = 0;
	SetUp(texData, Vector2<int>(sizeX, sizeY));
}

FrameBuffer::FrameBuffer(const TextureData& texFilteringData, const Vector2<int> size) {
	SetUp(texFilteringData, size);
}

void FrameBuffer::SetUp(const TextureData& texFilteringData, Vector2<int> size) {
	if (Context::window == nullptr) return;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	RenderedImage				= Texture(nullptr, size, texFilteringData);
	RenderedImage.name			= std::to_string((arch)this);
	RenderedImage._texData.name = RenderedImage.name;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderedImage.texture, 0);

	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RenderedImage.size.x, RenderedImage.size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}
void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Delete() {
	glDeleteRenderbuffers(1, &this->renderbuffer);
	glDeleteFramebuffers(1,  &this->framebuffer);
	RenderedImage.Delete();
}