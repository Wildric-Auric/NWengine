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

class Primitive {
    public:
    int width  = 0;
    int height = 0;

    void UpdateSize(float w, float h);
    virtual void Draw(){};
};

struct GLObjectData {
    uint32 VBO = 0;    
    uint32 EBO = 0;
    uint32 VAO = 0;
};

/**
 * @brief The QuadInternal class represents an internal implementation of a quad shape.
 */
class QuadInternal {
public:

    GLObjectData _gldat;

    static QuadInternal quadInstance; /**< The singleton instance of QuadInternal. */

    /**
     * @brief Constructs a new QuadInternal object.
     */
    QuadInternal();

    QuadInternal(int);

    /**
     * @brief Draws the quad.
     */
    void Draw();

    /**
     * @brief Deletes the quad.
     */
    void Delete();
};

class TriangleInternal {
    public:
    GLObjectData _gldat;
    static TriangleInternal triInstance;
    TriangleInternal();
    TriangleInternal(int);

    void Draw();
    void Delete();
};

/**
 * @brief The Quad class represents a quad shape.
 */
class Quad : public Primitive {
public:
    /**
     * @brief Constructs a new Quad object.
     * @param width The width of the quad.
     * @param height The height of the quad.
     */
    Quad(float width = 1.0f, float height = 1.0f);

    /**
     * @brief Draws the quad.
     */
    void Draw() override;
};

class Triangle : public Primitive {
    public:
    Triangle(float width = 1.0f, float height = 1.0f);
    void Draw() override;
};
