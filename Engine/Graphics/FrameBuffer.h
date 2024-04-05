#pragma once 
#include "Context.h"
#include "Texture.h"
#include "Maths.h"

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
    Texture textureBuffer; /**< The texture buffer associated with the framebuffer. */

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
    void SetUp(Vector2<int> size);

    /**
     * @brief Binds the framebuffer.
     */
    void Bind();

    /**
     * @brief Unbinds the framebuffer.
     */
    void Unbind();

    /**
     * @brief Deletes the framebuffer.
     */
    void Delete();
};