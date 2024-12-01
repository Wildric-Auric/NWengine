#include "GL/glew.h"

#include "FrameBuffer.h"
#include <string>

void FrameBuffer::SetUp(Vector2<int> size, MSAAValue msVal) {
	if (Context::window == nullptr) return;
	int colAtt = GL_COLOR_ATTACHMENT0;
	textureBuffer._size = size;
	textureBuffer._GPUGen(nullptr, TexChannelInfo::NW_RGB);
	textureBuffer.SetEdgesBehaviour(TexEdge::NW_CLAMP);
	textureBuffer.SetMinFilter(TexMinFilter::NW_MIN_LINEAR);
	textureBuffer.SetMaxFilter(TexMaxFilter::NW_LINEAR);
	
	NW_GL_CALL(glGenFramebuffers(1, &_framebuffer));
	Bind();
	if (msVal == MSAAValue::NW_MSx1) {
		NW_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, colAtt, GL_TEXTURE_2D, textureBuffer._glID, 0));
		Unbind();
		return;
	}

	NW_GL_CALL(glEnable(GL_MULTISAMPLE));
	_msaaVal = msVal;
	mstexure._size = size;
	mstexure._samplesNum = msVal;
	mstexure._GPUGen(TexChannelInfo::NW_RGB);
	NW_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, colAtt, GL_TEXTURE_2D_MULTISAMPLE, mstexure._glID, 0));
	resolveFbo = new FrameBuffer();
	resolveFbo->SetUp(size, NW_MSx1);
	textureBuffer.Clean();
	textureBuffer._glID = resolveFbo->textureBuffer._glID;
}

void FrameBuffer::Bind(RWFrameBuffer ro) {
	NW_GL_CALL(glBindFramebuffer(ro, _framebuffer));
}
void FrameBuffer::Unbind(RWFrameBuffer ro) {
	NW_GL_CALL(glBindFramebuffer(ro, 0));
}

void FrameBuffer::Delete() {
	NW_GL_CALL(glDeleteRenderbuffers(1, &this->_renderbuffer));
	NW_GL_CALL(glDeleteFramebuffers(1,  &this->_framebuffer));
	textureBuffer.Clean();
	mstexure.Clean();
	if (resolveFbo) 
		resolveFbo->Delete();	
	delete resolveFbo;
	resolveFbo = nullptr;
}

//TODO::UNfinished untested
void FrameBuffer::Blit(FrameBuffer* other) {
	Bind(NW_READ);
	if (other)
		other->Bind(NW_WRT);

	NW_GL_CALL(glBlitFramebuffer(0,0, textureBuffer._size.x, textureBuffer._size.y, 0,0, textureBuffer._size.x, textureBuffer._size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	
	Unbind();
}

void FrameBuffer::Resolve() {
	if (this->_msaaVal != NW_MSx1) 
		Blit(resolveFbo);
}
