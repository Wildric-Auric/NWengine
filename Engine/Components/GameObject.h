/**
 * @file GameObject.h
 * @brief Defines the GameObject and GameComponent classes.
 */

#pragma once
#include "Globals.h"
#include <map>
#include <string>

#define ADD_COMPONENT(str, type) if (type == #str ) return this->AddComponent<str>();

/**
 * @brief Function pointer type for draw callbacks. Mostly used internally.
 * @param data The data to be passed to the draw callback.
 * @return The result of the draw callback.
 */
typedef int (*DrawCallback)(void* data);

/**
 * @brief Base class for game components.
 */
class GameObject;

class GameComponent {
public:
    /**
     * @brief Get the type of the game component.
     * @return The type of the game component.
     */
    GameObject* attachedObject;

    static const char* GetType() { return "GameComponent"; }

    /**
     * @brief Update the game component. Does noting if not overriden.
     */
    inline virtual void Update() {};

    /**
     * @brief Start the game component. Does nothing if not overriden.
     */
    inline virtual void Start() {};

    /**
     * @brief OnAdd calls this during the component addition. Does nothing if not overriden.
     */
    inline virtual void OnAdd() {};

    /**
     * @brief OnDelete calls this during component deletion. Does nothing if not overriden.
     */
    inline virtual void OnDelete() {};

    /**
     * @brief Destructor for the game component.
     */
    inline virtual ~GameComponent() {};

    inline void SetGameObject(GameObject* obj) {
        attachedObject = (GameObject*)obj;
    }

    inline GameObject* GetGameObject() {
        return attachedObject;
    }
};

/**
 * @brief Class representing a game object.
 */
class GameObject {
private:
    static int numberOfGameObjects; //Unused.

public:
    std::map<std::string, GameComponent*> components; /**< Map of components attached to the game object. Internal member */
    std::string name = "new GameObject"; /**< The name of the game object. */
    uint32 tag = 0; /**< A tag of the game object. Can be used for different purposes. */
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
     * @brief Destructor for GameObject. Will deallocate component resources.
     */
    ~GameObject();

    /**
     * @brief Will call the function callack set to draw on the current framebuffer.
     * @return The layer in which the game object has been drawn.
     */
    int Draw(); 

    /**
     * @brief Set the draw callback which will be called by Draw()
     * @param callback The draw callback to set.
     */
    void SetDrawCallback(DrawCallback callback);

    /**
     * @brief Delete a component attached to the gameobject,
     * if no component component is found, nothing is done.
     * @param typeName The type name of the component to delete. Example: "Sprite"
     */
    void DeleteComponent(std::string typeName);

    /**
     * @brief Delete all components from the game object.
     */
    void DeleteComponents();

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
        ptr->OnAdd(); 
        return ptr;
    };

    /**
     * @brief Add a component of the secified type name to the game object.
     * @tparam The component type name that should be attached; example: "Transform"
     * @return A pointer to the component.
     */

    GameComponent* AddComponent(std::string type);
    /**
     * @brief Get a component of the specified type.
     * @param type The type of the component.
     * @return A pointer to the component, or nullptr if the component does not exist.
     */
    GameComponent* GetComponent(const std::string& type);
    /**
     * @brief Delete a component of the specified type. 
     * If it is not found, this function does nothing.
     * @tparam T The type of the component.
     */
    template<typename T>
    void DeleteComponent() {
        auto iter = components.find(T::GetType());
        if (iter == components.end()) return;
        iter->second->OnDelete();
        delete components[T::GetType()];
        components.erase(T::GetType());
    }
};
