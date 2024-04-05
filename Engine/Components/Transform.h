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
    GameObject* attachedObj; /**< The game object attached to the Transform component. */

    /**
     * @brief Default constructor for the Transform class.
     */
    Transform() {};

    /**
     * @brief Constructor for the Transform class.
     * @param go The game object to attach the Transform component to.
     */
    Transform(GameObject* go);

    /**
     * @brief Serializes the Transform component data.
     * @param data The file stream to write the serialized data to.
     * @param offset The offset in the file stream to start writing the serialized data.
     * @return The number of bytes written to the file stream.
     */
    int Serialize(std::fstream* data, int offset) override;

    /**
     * @brief Deserializes the Transform component data.
     * @param data The file stream to read the serialized data from.
     * @param offset The offset in the file stream to start reading the serialized data.
     * @return The number of bytes read from the file stream.
     */
    int Deserialize(std::fstream* data, int offset) override;

    /**
     * @brief Sets the game object attached to the Transform component.
     * @param go The game object to attach.
     */
    void SetGameObject(void* go) override;

    /**
     * @brief Gets the game object attached to the Transform component.
     * @return The game object attached to the Transform component.
     */
    void* GetGameObject() override;

    static std::map<GameObject*, Transform> componentList; /**< The list of game objects with Transform components. */
};
