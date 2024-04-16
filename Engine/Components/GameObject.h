/**
 * @file GameObject.h
 * @brief Defines the GameObject class and its related components.
 */

#pragma once
#include "Globals.h"
#include "Serialization.h"

#include <map>

/**
 * @brief Macro to add a component to the GameObject.
 * @param str The name of the component.
 * @param type The type of the component.
 */
#define ADD_COMPONENT(str, type) if (type == #str ) return this->AddComponent<str>();

/**
 * @brief Function pointer type for draw callbacks.
 * @param data The data to be passed to the draw callback.
 * @return The result of the draw callback.
 */
typedef int (*DrawCallback)(void* data);

/**
 * @brief Base class for game components.
 */
class GameComponent: public Serialized {
public:
    /**
     * @brief Get the type of the game component.
     * @return The type of the game component.
     */
    static std::string GetType() { return "GameComponent"; }

    /**
     * @brief Update the game component.
     */
    virtual void Update() {};

    /**
     * @brief Start the game component.
     */
    virtual void Start() {};

    /**
     * @brief Set the GameObject associated with the game component.
     * @param go The GameObject to set.
     */
    virtual void SetGameObject(void* go) {};

    /**
     * @brief Get the GameObject associated with the game component.
     * @return The GameObject associated with the game component.
     */
    virtual void* GetGameObject() { return nullptr; };

    /**
     * @brief Destructor for the game component.
     */
    virtual ~GameComponent() {};
};

/**
 * @brief Class representing a game object.
 */
class GameObject : public Serialized {
private:
    static int numberOfGameObjects;

public:
    std::map<std::string, GameComponent*> components; /**< Map of components attached to the game object. */
    std::string name = "new GameObject"; /**< The name of the game object. */
    uint32 id = 0; /**< The identifier of the game object. */
    DrawCallback _drawProc = nullptr; /**< The draw callback for the game object. */

    /**
     * @brief Default constructor for GameObject.
     */
    GameObject();

    /**
     * @brief Copy constructor for GameObject.
     * @param other The GameObject to copy.
     */
    GameObject(const GameObject& other);

    /**
     * @brief Destructor for GameObject.
     */
    ~GameObject();

    /**
     * @brief Draw the game object.
     * @return The layer in which the game object has been drawn.
     */
    int Draw();

    /**
     * @brief Set the draw callback for the game object.
     * @param callback The draw callback to set.
     */
    void SetDrawCallback(DrawCallback callback);

    /**
     * @brief Delete a component from the game object.
     * @param typeName The type name of the component to delete.
     */
    void DeleteComponent(std::string typeName);

    /**
     * @brief Delete all components from the game object.
     */
    void DeleteComponents();

    /**
     * @brief Serialize the game object.
     * @param data The file stream to serialize to.
     * @param offset The offset in the file stream.
     * @return The number of bytes written.
     */
    int Serialize(std::fstream* data, int offset);

    /**
     * @brief Deserialize the game object.
     * @param data The file stream to deserialize from.
     * @param offset The offset in the file stream.
     * @return The number of bytes read.
     */
    int Deserialize(std::fstream* data, int offset);

    /**
     * @brief Get a component of the specified type.
     * @tparam T The type of the component.
     * @return A pointer to the component, or nullptr if the component does not exist.
     */
    template<typename T>
    T* GetComponent() {
        T* component = nullptr;
        if (this->components.find(T::GetType()) == this->components.end()) return component;

        component = (T*)this->components[T::GetType()];
        return component;
    };

    /**
     * @brief Add a component of the specified type to the game object.
     * @tparam T The type of the component.
     * @return A pointer to the added component.
     */
    template<typename T>
    T* AddComponent() {
        std::map<std::string, GameComponent*>::iterator temp = components.find(T::GetType());
        if (temp != components.end()) return (T*)(*&temp)->second;
        T* ptr = new T(this);
        components.insert(std::pair<std::string, GameComponent*>( T::GetType(), ptr ));
        return ptr;
    };

    GameComponent* AddComponent(std::string type);
    /**
     * @brief Get a component of the specified type.
     * @param type The type of the component.
     * @return A pointer to the component, or nullptr if the component does not exist.
     */
    GameComponent* GetComponent(std::string type);

    /**
     * @brief Delete a component of the specified type.
     * @tparam T The type of the component.
     */
    template<typename T>
    void DeleteComponent() {
        if (components.find(T::GetType()) == components.end()) return;
        delete components[T::GetType()];
        components.erase(T::GetType());
    }
};