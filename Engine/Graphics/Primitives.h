#pragma once
#include "vector"
#include "Globals.h"

/**
 * @brief The Primitives class represents a collection of primitive shapes.
 */
class Primitives {
public:
    /**
     * @brief Initializes the Primitives class.
     */
    static void Init();

    /**
     * @brief Destroys the Primitives class.
     */
    static void Destroy();
};

/**
 * @brief The QuadInternal class represents an internal implementation of a quad shape.
 */
class QuadInternal {
public:
    uint32 VBO = 0; /**< The vertex buffer object ID. */
    uint32 EBO = 0; /**< The element buffer object ID. */
    uint32 VAO = 0; /**< The vertex array object ID. */
    static QuadInternal quadInstance; /**< The singleton instance of QuadInternal. */

    /**
     * @brief Constructs a new QuadInternal object.
     */
    QuadInternal();

    /**
     * @brief Constructs a new QuadInternal object with a parameter.
     * @param a The parameter value.
     */
    QuadInternal(int a);

    /**
     * @brief Draws the quad.
     */
    void Draw();

    /**
     * @brief Deletes the quad.
     */
    void Delete();
};

/**
 * @brief The Quad class represents a quad shape.
 */
class Quad {
public:
    int width = 0; /**< The width of the quad. */
    int height = 0; /**< The height of the quad. */

    /**
     * @brief Updates the size of the quad.
     * @param width The new width of the quad.
     * @param height The new height of the quad.
     */
    void UpdateSize(float width, float height);

    /**
     * @brief Constructs a new Quad object.
     * @param width The width of the quad.
     * @param height The height of the quad.
     */
    Quad(float width = 1.0f, float height = 1.0f);

    /**
     * @brief Draws the quad.
     */
    void Draw();
};
