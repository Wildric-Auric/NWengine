#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"
#include "Primitives.h"
#include "ComponentTypes.h"

/**
 * @brief The Transform class represents the transformation of a game object.
 */

struct TriangleCoord {
    fVec2 bl;
    fVec2 br = fVec2(100.0f,0.0f);
    fVec2 up = fVec2(50.0,100.0);
};

class TriangleRenderer: public GameComponent {
public:
    /**
     * @brief GetType returns the type of the Transform component.
     * @return The type of the Transform component.
     */
    NW_ST_GET_TYPE_IMPL(TriangleRenderer);
    static int TriangleDrawCallback(void*);
    TriangleCoord _coord;
    Triangle _triangle;
    TriangleRenderer() = default; 
    TriangleRenderer(GameObject*);

    void OnAdd() override;
    void SetBaseLeft(const fVec2&);
    void SetBaseRight(const fVec2&);
    void SetTop(const fVec2&);
    void SetCoord(const TriangleCoord&);
};
