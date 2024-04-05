#pragma once
#include"Globals.h"
#include"Asset.h"

#include<unordered_map>

typedef void* TextureIdentifierPtr;

/**
 * @brief Struct representing the identifier of a texture.
 */
struct TextureIdentifier {
    std::string name; /**< The name of the texture. */
    uint8 alpha; /**< The alpha value of the texture. */

    /**
     * @brief Overloaded equality operator for comparing TextureIdentifier objects.
     * @param other The other TextureIdentifier object to compare with.
     * @return True if the TextureIdentifier objects are equal, false otherwise.
     */
    bool operator==(const TextureIdentifier& other) const {
        return alpha == other.alpha && name == other.name;
    }
};

template <>
struct std::hash<TextureIdentifier>
{
    /**
     * @brief Hash function for TextureIdentifier objects.
     * @param t The TextureIdentifier object to hash.
     * @return The hash value of the TextureIdentifier object.
     */
    std::size_t operator()(const TextureIdentifier& t) const
    {
        std::size_t res = 0;
        hashCombine(res, t.name);
        hashCombine(res, t.alpha);
        return res;
    }
};

/**
 * @brief Enumeration of texture channel information.
 */
enum TexChannelInfo {
    NW_R = 0x1903, /**< Red channel. */
    NW_RGB = 0x1907, /**< RGB channels. */
    NW_RGBA = 0x1908 /**< RGBA channels. */
};

/**
 * @brief Enumeration of texture minification filters.
 */
enum TexMinFilter {
    NW_MIN_LINEAR = 0x2601, /**< Linear minification filter. */
    NW_MIN_NEAREST = 0x2600, /**< Nearest minification filter. */
    NW_NEAREST_MIPMAP_NEAREST = 0x2700, /**< Nearest minification filter with nearest mipmap. */
    NW_NEAREST_MIPMAP_LINEAR = 0x2702 /**< Nearest minification filter with linear mipmap. */
};

/**
 * @brief Enumeration of texture magnification filters.
 */
enum TexMaxFilter {
    NW_LINEAR = 0x2601, /**< Linear magnification filter. */
    NW_NEAREST = 0x2600 /**< Nearest magnification filter. */
};

/**
 * @brief Enumeration of texture edge behaviors.
 */
enum TexEdge {
    NW_REPEAT = 0x2901, /**< Repeat texture edge behavior. */
    NW_CLAMP = 0x2900 /**< Clamp texture edge behavior. */
};

/**
 * @brief Class representing a texture asset.
 */
class Texture : public Asset {
public:
    uint32 _glID = 0; /**< The OpenGL ID of the texture. Public for the framebuffer. */
    Vector2<int> _size; /**< The size of the texture. */
    bool _hasMipMap; /**< Flag indicating whether the texture has mipmaps. */

    Texture() = default;

    /**
     * @brief Cleans up the texture.
     */
    void Clean() override;

    /**
     * @brief Binds the texture to a specified texture slot.
     * @param slot The texture slot to bind the texture to.
     */
    void Bind(uint32 slot = 0);

    /**
     * @brief Generates the texture on the GPU.
     * @param data The texture data.
     * @param channelInfo The channel information of the texture.
     */
    void _GPUGen(uint8* data, TexChannelInfo channelInfo);

    /**
     * @brief Generates mipmaps for the texture.
     */
    void GenMipMap();

    /**
     * @brief Sets the minification filter for the texture.
     * @param minFilter The minification filter to set.
     */
    void SetMinFilter(TexMinFilter minFilter);

    /**
     * @brief Sets the magnification filter for the texture.
     * @param maxFilter The magnification filter to set.
     */
    void SetMaxFilter(TexMaxFilter maxFilter);

    /**
     * @brief Sets the edge behavior for the texture.
     * @param edge The edge behavior to set.
     */
    void SetEdgesBehaviour(TexEdge edge);

    /**
     * @brief Gets the texture from the cache based on the identifier.
     * @param identifier The identifier of the texture.
     * @return The loaded texture from the cache.
     */
    Asset* GetFromCache(void* identifier) override;

    /**
     * @brief Loads the texture from a file.
     * @param path The path to the file.
     * @param identifier The identifier of the texture.
     * @return The loaded texture.
     */
    Asset* LoadFromFile(const char* path, TextureIdentifierPtr identifier) override;

    /**
     * @brief Loads the texture from a buffer.
     * @param buffer The buffer containing the texture data.
     * @param data The additional data for loading the texture.
     * @return The loaded texture.
     */
    Asset* LoadFromBuffer(void* buffer, void* data) override;

    NW_DECL_RES_LIST(TextureIdentifier, Texture)
};
