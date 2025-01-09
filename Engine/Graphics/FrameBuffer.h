#pragma once 
#include "Context.h"
#include "Texture.h"
#include "Maths.h"


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
    FrameBuffer* resolveFbo = nullptr;
    MSAAValue _msaaVal = NW_MSx1;
    std::vector<FrameBufferAttachment> attachments; /**< The framebuffer attachments*/     

    /**
     * @brief Default constructor for the FrameBuffer class.
     */
    FrameBuffer() = default;

    /**
     * @brief Sets up the framebuffer with the specified size.
     * 
     * @param size The size of the framebuffer.
     * 
     * @note Calling this method multiple times without deleting the framebuffer may result in a memory leak.
     */
    void SetUp(Vector2<int> size, MSAAValue msVal = NW_MSx1);
    
    void AddAttachment(iVec2 size);

    FrameBufferAttachment& GetAtt(int i = 0);
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
