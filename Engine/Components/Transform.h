#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"

/**
 * @brief The Transform class represents the transformation of a game object.
 */
class Transform : public GameComponent {
public:
    /**
     * @brief GetType returns the type of the Transform component.
     * @return The type of the Transform component.
     */
    static std::string GetType() { return "Transform"; };

    fVec2 position = fVec2(0.0f,0.0f); /**< The position of the game object. */
    fVec2 scale    = fVec2(1.0f, 1.0f); /**< The scale of the game object. */
    float rotation = 0.0f; /**< The rotation of the game object. */

    /**
     * @brief Default constructor for the Transform class.
     */
    Transform() {};

    /**
     * @brief Constructor for the Transform class.
     * @param go The game object to attach the Transform component to.
     */
    Transform(GameObject* go);

    fVec2& Translate(const fVec2&);

    fVec2& Scale(const fVec2&);

    fVec2& Lerp(const fVec2& start, const fVec2& end, const fVec2& ratio);

    float Rotate(const float);

    void SetPosition(const fVec2&);

    void SetScale(const fVec2&);

    void SetRotation(const float);

    fVec2 GetPosition();

    fVec2 GetScale();

    float GetRotation();
};
