#pragma once
#include <vector>
#include "Context.h"
#include "Image.h"
#include "Maths.h"
#include "Texture.h"

enum MSAAValue {
	NW_MSx1	 = 1,
	NW_MSx2	 = 2,
	NW_MSx4	 = 4,
	NW_MSx8	 = 8,
	NW_MSx16 = 16,
	NW_MSx32 = 32,
};

enum RWFrameBuffer {
	NW_READ = 0x8CA8,
	NW_WRT	= 0x8CA9,
	NW_RW	= 0x8D40,

};

/**
 * @brief The FrameBufferAttachment class represents an attachment of the framebuffer.
 *
 */
class FrameBufferAttachment {
  public:
	void*	  owner; /**< The framebuffer owner of this attachment. */
	MSTexture msTex;
	Texture	  tex;
    void      SetUp(iVec2 size, MSAAValue msVal, TexType_Exp type, uint8 num);
	void	  Clean();
};

/**
 * @brief The FrameBuffer class represents a framebuffer object.
 *
 * This class provides functionality to create, bind, unbind, and delete a framebuffer object.
 * It also allows setting up the framebuffer with a specified size.
 */
class FrameBuffer {
  private:
	uint32 _framebuffer	 = 0;
	uint32 _renderbuffer = 0;
  public:
	static FrameBuffer*				   _current;
	FrameBuffer*					   resolveFbo = nullptr;
	MSAAValue						   _msaaVal	  = NW_MSx1;
	std::vector<FrameBufferAttachment> attachments; /**< The framebuffer attachments*/
	static FrameBuffer* GetCurrent();
	FrameBuffer() = default;
	void CopyFramebufferToCPU(Image* img, int attIndex = 0);
	void SetUp(Vector2<int> size, MSAAValue msVal, TexType_Exp type);
	void AddAttachment(iVec2 size, TexType_Exp type);
	uint32 GetAttNum();
	FrameBufferAttachment& GetAtt(int i = 0);
	void ClearAttachment(int i, const fVec4& clearColor);
	bool CheckCompleteness();
	void Bind(RWFrameBuffer ro = NW_RW);
	void Blit(FrameBuffer* other);
	void Resolve();
	void Unbind(RWFrameBuffer ro = NW_RW);
	void Delete();
	void GenDepthStencilBuffer();
};
