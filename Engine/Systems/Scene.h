#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include<list>

/**
 * @brief Represents a scene in the game.
 */
class Scene {
public:
    bool _shouldDelObj = 0; /**< Internal flag that holds if currently iterated on gameobject during update should be deleted. see DestroyCurrentObj() for details*/

    std::string name; /**< The name of the scene. */
    std::list<GameObject> sceneObjs; /**< The list of game objects in the scene. */
    std::list<Sprite*> drawList; /**< The list of sprites to be drawn in the scene. */

    /**
     * @brief Adds a new empty GameObject to the scene objects container.
     * @return The reference to the newly added GameObject.
     */
    GameObject& AddObject();

    /**
     * @brief Deletes a GameObject from the scene objects container by index.
     * @param index The index of the GameObject to delete.
     */
    void DeleteObject(uint32 index);

    /**
     * @brief Deletes a GameObject from the scene objects container by name.
     * @param name The name of the GameObject to delete.
     */
    void DeleteObject(std::string name);

    /**
     * @brief Deletes a GameObject from the scene objects container by iterator.
     * @param it The iterator of the object to delete.
     * @return Return value of std::list::erase()
    */
    std::list<GameObject>::iterator DeleteObject(std::list<GameObject>::iterator it);

    /**
     * @brief This function is intended to be called within object (to-delete) component (like scripts), as DeleteObject() will produce invalidation
     * internally.
     */
    void DeleteCurrentObj();


    /**
     * @brief Gets a GameObject in the scene by name.
     * @param name The name of the GameObject to get.
     * @return A pointer to the GameObject if found, nullptr otherwise.
     */
    GameObject* GetGameObject(std::string name);

    /**
     * @brief Gets a GameObject in the scene by position.
     * @param position The position of the GameObject to get.
     * @return A pointer to the GameObject if found, nullptr otherwise.
     */
    GameObject* GetGameObject(const uint32& position);

    /**
     * @brief Gets the last object added to the scene.
     * @return A pointer to the last GameObject added to the scene if available, nullptr otherwise.
     */
    GameObject* GetGameObject();

    /**
     * @brief Deprecated. Sorts the scene objects container.
     * This method is no longer used as objects are kept sorted at each insertion.
     */
    void SortScene();

    /**
     * @brief Adds a sprite to the draw list.
     * @param sprite The sprite to add.
     */
    void Render(Sprite* sprite);

    /**
     * @brief Iterates over the draw list and deletes sprites that are not rendered.
     * If a sprite is not rendered, it will be deleted. Otherwise, the action is taken only at the next scene draw.
     */
    void ForceRenderStop();

    /**
     * @brief Refinds a position to insert a sprite in the draw list.
     * This method is called internally when the layer of a sprite changes.
     * @param sprite The sprite to rearrange.
     */
    void Rearrange(Sprite* sprite);

    /**
     * @brief Renames a GameObject.
     * This method is mostly used internally and can be used by the user to change the name of an object.
     * @param newName The new name for the GameObject.
     * @param obj The GameObject to rename.
     * @return The new name of the GameObject.
     */
    const std::string& Rename(const std::string& newName, GameObject* obj);

    /**
     * @brief Constructs a Scene object with the given path.
     * @param path The path of the scene file.
     */
    Scene(const std::string& path);

    /**
     * @brief Destroys the Scene object.
     */
    ~Scene();

    /**
     * @brief Loads the scene from the file with the same name as the path.
     */
    void LoadScene();

    /**
     * @brief Sets the scene as the current scene.
     * The current scene will be updated and drawn during the engine update.
     */
    void MakeCurrent();

    /**
     * @brief Checks if the scene is the current scene.
     * @return True if the scene is the current scene, false otherwise.
     */
    bool IsCurrent();

    /**
     * @brief Initializes the scene.
     * This method should be called once before the update.
     */
    void Start();

    /**
     * @brief Updates the scene.
     * This method should be called every frame.
     * It is called within the default NWengine update.
     */
    void Update();

    /**
     * @brief Sets up the needed static variables and other things.
     */
    void SetUp();

    /**
     * @brief Draws all the scene objects.
     * This method is called within the default NWengine update.
     */
    void Draw();

    /**
     * @brief Serializes the scene and saves it in a file with the same name.
     */
    void Save();

    /**
     * @brief Sets the path of the scene.
     * This method changes the "name" member to the given path.
     * @param path The new path for the scene.
     */
    void SetPath(const std::string& path);

    /**
     * @brief Creates a new scene and pushes it to a container.
     * @param path The path of the new scene.
     * @return The reference to the newly created scene.
     */
    static Scene& CreateNew(const std::string& path);

    /**
     * @brief Gets a scene by its path.
     * If multiple scenes have the same path, it returns one of them; order or determinism is not guaranteed.
     * @param path The path of the scene.
     * @return A pointer to the scene if found, nullptr otherwise.
     */
    static Scene* GetScene(const std::string& path);

    /**
     * @brief Deletes a scene by its path.
     * @param path The path of the scene to delete.
     * @return True if the scene is found and deleted, false otherwise.
     */
    static bool DeleteScene(const std::string& path);

    /**
     * @brief Destroys the current scene.
     */
    static void Destroy();

    /**
     * @brief Updates the active scene.
     */
    static void UpdateActiveScene();

    /**
     * @brief Gets the current scene.
     * @return A pointer to the current scene if available, nullptr otherwise.
     */
    static Scene* GetCurrent();

    static std::list<Scene> _scenes; /**< The list of all scenes. */
    static Scene* currentScene; /**< The pointer to the current scene. */
};