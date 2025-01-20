#include "GL/glew.h"
#include "FrameBuffer.h"

FrameBuffer* FrameBuffer::GetCurrent() {
    return _current;
}

FrameBuffer* FrameBuffer::_current = 0;

void FrameBufferAttachment::SetUp(iVec2 size, MSAAValue msVal, uint8 num) {
	tex._size = size;
	tex._GPUGen(nullptr, TexChannelInfo::NW_RGB);
	tex.SetEdgesBehaviour(TexEdge::NW_CLAMP);
	tex.SetMinFilter(TexMinFilter::NW_MIN_LINEAR);
	tex.SetMaxFilter(TexMaxFilter::NW_LINEAR);
    if (msVal != NW_MSx1) {
	    msTex._size = size;
	    msTex._samplesNum = msVal;
	    msTex._GPUGen(TexChannelInfo::NW_RGB);
        tex.Clean();
        tex._glID = ((FrameBuffer*)owner)->resolveFbo->GetAtt(num).tex._glID;
	    NW_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D_MULTISAMPLE, msTex._glID, 0));
        return;
     }
	 NW_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D, tex._glID, 0));
}

void FrameBufferAttachment::Clean() {
    tex.Clean();
	msTex.Clean();
}


FrameBufferAttachment& FrameBuffer::GetAtt(int i) {
    return attachments[i];
}

void FrameBuffer::ClearAttachment(int i, const fVec4& clearColor) {
    NW_GL_CALL(glClearBufferfv(GL_COLOR, i, &clearColor.x));
}

void FrameBuffer::AddAttachment(iVec2 size) {
    uint8 num = attachments.size();
    if (_msaaVal != MSAAValue::NW_MSx1)
        resolveFbo->AddAttachment(size);

    Bind();
    attachments.push_back({});
	attachments.back().owner = this;
    attachments.back().SetUp(size, _msaaVal, num);

	std::vector<uint32> t(num + 1);
	for (int i = 0; i < num + 1; i++) { t[i] = GL_COLOR_ATTACHMENT0 + i;}
	glDrawBuffers(num + 1, t.data());

    Unbind();
}

uint32 FrameBuffer::GetAttNum() {
   return this->attachments.size(); 
}

void FrameBuffer::SetUp(Vector2<int> size, MSAAValue msVal) {
	if (Context::window == nullptr) return;
    if (msVal != NW_MSx1) {
	    resolveFbo = new FrameBuffer();
	    resolveFbo->SetUp(size, NW_MSx1);
	    NW_GL_CALL(glEnable(GL_MULTISAMPLE));
    }
	NW_GL_CALL(glGenFramebuffers(1, &_framebuffer));
	_msaaVal = msVal;

	Bind();
    AddAttachment(size);
    Unbind();
}

void FrameBuffer::Bind(RWFrameBuffer ro) {
	NW_GL_CALL(glBindFramebuffer(ro, _framebuffer));
    _current = this;
}
void FrameBuffer::Unbind(RWFrameBuffer ro) {
	NW_GL_CALL(glBindFramebuffer(ro, 0));
    _current = 0;
}

void FrameBuffer::Delete() {
	NW_GL_CALL(glDeleteRenderbuffers(1, &this->_renderbuffer));
	NW_GL_CALL(glDeleteFramebuffers(1,  &this->_framebuffer));
    for (auto& att : attachments)
       att.Clean(); 

	if (resolveFbo) 
		resolveFbo->Delete();	
	delete resolveFbo;
	resolveFbo = nullptr;
}

void FrameBuffer::Blit(FrameBuffer* other) {
	Bind(NW_READ);
	if (other)
		other->Bind(NW_WRT);
    
    for (int i = 0; i < attachments.size(); ++i) {
        FrameBufferAttachment& att = GetAtt(i);
        NW_GL_CALL(glReadBuffer(GL_COLOR_ATTACHMENT0 + i));
        NW_GL_CALL(glDrawBuffer(GL_COLOR_ATTACHMENT0 + i));
	    NW_GL_CALL(glBlitFramebuffer(0,0, att.tex._size.x, att.tex._size.y, 0,0, att.tex._size.x, att.tex._size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST));
    }
	
	Unbind();
}

void FrameBuffer::Resolve() {
	if (this->_msaaVal != NW_MSx1) 
		Blit(resolveFbo);
}
