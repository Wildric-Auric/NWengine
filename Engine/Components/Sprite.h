/**
 * @file Sprite.h
 * @brief Defines the Sprite class, which represents a game sprite.
 */

#pragma once

#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Image.h"
#include "Primitives.h"

/**
 * @brief Enumerates the batch types for sprites.
 * @note static batching is to be handled manually, updating states related needed by
 * a batch won't update those that were registered when the object was pushed.
 */
enum BatchType {
    UNBATCHED,       /**< Unbatched sprite */
    STATIC_BATCH = 1, /**< Static batched sprite */
    DYNAMIC_BATCH = 2 /**< Dynamic batched sprite */
};

/**
 * @brief Represents a game sprite.
 */
class Sprite : public GameComponent {
private:
public:
    static std::string GetType() { return "Sprite"; };
    TextureIdentifier _texId{};
    Texture* texture = nullptr;

    BatchType _isBatched = BatchType::UNBATCHED;
    int  _lastSortingLayer = 0;
    bool _shouldDraw = 1;
    bool _isRendered = 1;

    fVec3 vertexAttributes; /**< Vertex attributes of the sprite */
    Shader* shader = nullptr;
    Quad container; /**< Container of the sprite */
    int sortingLayer = 0; /**< Sorting layer of the sprite */
    double zbuffer = 1.0; /**< Z-buffer value of the sprite */

    /**
     * @brief Default constructor for the Sprite class.
     */
    Sprite() = default;

    /**
     * @brief Constructor for the Sprite class.
     * @param go The GameObject to attach the sprite to.
     */
    Sprite(GameObject* go);

    /**
     * @brief Destructor for the Sprite class.
     */
    ~Sprite();

    void SetSize(const fVec2&);

    /**
     * @brief Sets the texture of the sprite from a file path.
     * @param path The file path of the texture.
     * @param alpha Flag indicating whether the texture has an alpha channel.
     */
    void SetTexture(std::string path, bool alpha = 1);
        
    /**
     * @brief Sets the texture of the sprite from an Image object.
     * @param image The Image object representing the texture.
     * @param texId The identifier of the texture.
     */
    void SetTexture(const Image* image, TextureIdentifierPtr texId);

    /**
     * @brief Sets the texture of the sprite.
     * @param tex The texture to set.
     */
    void SetTexture(Texture* tex);

    /**
     * @brief Sets the shader of the sprite from a file path.
     * @param path The file path of the shader.
     */
    void SetShader(std::string path);

    /**
     * @brief Sets the shader of the sprite from a ShaderText object.
     * @param st The ShaderText object representing the shader.
     * @param id The identifier of the shader.
     */
    void SetShader(const ShaderText& st, ShaderIdentifier* id);

    /**
     * @brief Sets the shader of the sprite.
     * @param shader The shader to set.
     */
    void SetShader(Shader* shader);

    /**
     * @brief Sets the sorting layer of the sprite.
     * @param order The sorting order of the sprite.
     */
    void SetSortingLayer(int order);

    /**
     * @brief Renders the sprite.
     */
    void Render();

    /**
     * @brief Stops rendering the sprite.
     */
    void StopRendering();

    /**
     * @brief Batches the sprite.
     * @param type The batch type to use.
     */
    void Batch(BatchType type = BatchType::DYNAMIC_BATCH);

    /**
     * @brief Unbatches the sprite.
     */
    void UnBatch();

    /**
     * @brief Updates the sprite.
     */
    void Update() override;

    /**
     * @brief Default draw callback for sprites.
     * @param data The data to pass to the draw callback.
     * @return The result of the draw callback.
     */
    static int DefaultSpriteDrawCallback(void* data);
};

class BatchExtra : public GameComponent {
    public:
    std::vector<float> rawData;
    std::vector<int> relativeIndex;
	inline BatchExtra(GameObject* go) {
		attachedObject = go;
	}
    //void Start();
    //void Update();  
    void AddAttribute(int numOfFloat);
    void SetAttribute(int index, void* data);
    void* GetData(int num);
    bool IsCompatible(BatchExtra* other);
    bool IsCompatible(int* container, int size);
    template<typename T>
    void SetAttribute(int index, const T& data) {
        SetAttribute(index,(void*)&data);
    }
    template<typename T>
    T GetData(int num) {
        return *(T*)GetData(num);
    }
};
