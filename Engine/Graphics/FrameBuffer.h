#pragma once 
#include "Context.h"
#include "Texture.h"
#include "Maths.h"
#include "Image.h"


enum MSAAValue {
    NW_MSx1 = 1,
    NW_MSx2 = 2,
    NW_MSx4 = 4,
    NW_MSx8 = 8,
    NW_MSx16 = 16,
    NW_MSx32 = 32,
};

enum RWFrameBuffer {
    NW_READ = 0x8CA8,
    NW_WRT = 0x8CA9,
    NW_RW = 0x8D40,

};

/**
 * @brief The FrameBufferAttachment class represents an attachment of the framebuffer.
 * 
 */
class FrameBufferAttachment {
    public:
    void* owner; /**< The framebuffer owner of this attachment. */
    MSTexture msTex;
    Texture   tex;
    void SetUp(iVec2 size, MSAAValue msVal, uint8 num);
    void Clean();
};

/**
 * @brief The FrameBuffer class represents a framebuffer object.
 * 
 * This class provides functionality to create, bind, unbind, and delete a framebuffer object.
 * It also allows setting up the framebuffer with a specified size.
 */
class FrameBuffer {
private:
    uint32 _framebuffer  = 0; /**< The framebuffer ID. */
    uint32 _renderbuffer = 0; /**< The renderbuffer ID. */
public: 
    static FrameBuffer* _current;
    FrameBuffer* resolveFbo = nullptr;
    MSAAValue _msaaVal = NW_MSx1;
    std::vector<FrameBufferAttachment> attachments; /**< The framebuffer attachments*/     
    /**
      * @brief Get the currently bound framebuffer.
      */
    static FrameBuffer* GetCurrent();

    /**
     * @brief Default constructor for the FrameBuffer class.
     */
    FrameBuffer() = default;

    void CopyFramebufferToCPU(Image* img, int attIndex = 0);
    /**
     * @brief Sets up the framebuffer with the specified size.
     * @param size The size of the framebuffer.
     * @note Calling this method multiple times without deleting the framebuffer may result in a memory leak.
     */
    void SetUp(Vector2<int> size, MSAAValue msVal = NW_MSx1);

    /**
     * @brief Add an attachment to the the framebuffer.
     * @param size The size of the new attachment.
     * @note Multisample value is the same for all attachments.
     */
    void AddAttachment(iVec2 size);

    /**
      * @brief Get the number of attachments of the fbo.
    */
    uint32 GetAttNum();

    /**
     * @brief Get an attachment from the previously added attachments.
     * @param i The index of the attachment.
     * @return a reference to the attachment.
     */
    FrameBufferAttachment& GetAtt(int i = 0);

    /**
     * @brief Clears a specific attachement
     * @param i The index of the attachment.
     * @param clearColor The color which is used to clear the i'th attachment
     * @note The framebuffer should be bound before this operation
     * the result is otherwise undefined.
    */
    void ClearAttachment(int i, const fVec4& clearColor);

    
    /**
     * @brief Binds the framebuffer.
     */
    void Bind(RWFrameBuffer ro = NW_RW);


    void Blit(FrameBuffer* other);

    /**
     * @brief Resolves MSAA attachment; result is copied to textureBuffer
     * @note  Using textureBuffer without calling resolve previously is undefined.
     */
    void Resolve();

    /**
     * @brief Unbinds the framebuffer.
     */

    void Unbind(RWFrameBuffer ro = NW_RW);

    /**
     * @brief Deletes the framebuffer.
     */
    void Delete();
};
