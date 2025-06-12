#pragma once
#include "Asset.h"
#include "Globals.h"

#include <string>
#include <unordered_map>

typedef void* TextureIdentifierPtr;

/**
 * @brief Struct representing the identifier of a texture.
 */
struct TextureIdentifier {
	std::string name;  /**< The name of the texture. */
	uint8		alpha; /**< The alpha value of the texture. */

	/**
	 * @brief Overloaded equality operator for comparing TextureIdentifier objects.
	 * @param other The other TextureIdentifier object to compare with.
	 * @return True if the TextureIdentifier objects are equal, false otherwise.
	 */
	bool operator==(const TextureIdentifier& other) const { return alpha == other.alpha && name == other.name; }
};

template <> struct std::hash<TextureIdentifier> {
	/**
	 * @brief Hash function for TextureIdentifier objects.
	 * @param t The TextureIdentifier object to hash.
	 * @return The hash value of the TextureIdentifier object.
	 */
	std::size_t operator()(const TextureIdentifier& t) const {
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
	NW_R	= 0x1903, /**< Red channel. */
	NW_RGB	= 0x1907, /**< RGB channels. */
	NW_RGBA = 0x1908  /**< RGBA channels. */
};

/**
 * @brief Enumeration of texture minification filters.
 */
enum TexMinFilter {
	NW_MIN_LINEAR			  = 0x2601, /**< Linear minification filter. */
	NW_MIN_NEAREST			  = 0x2600, /**< Nearest minification filter. */
	NW_NEAREST_MIPMAP_NEAREST = 0x2700, /**< Nearest minification filter with nearest mipmap. */
	NW_NEAREST_MIPMAP_LINEAR  = 0x2702, /**< Nearest minification filter with linear mipmap. */
	NW_LINEAR_MIPMAP_NEAREST  = 0x2701,
	NW_LINEAR_MIPMAP_LINEAR	  = 0x2703
};

enum RWImage {
	NW_IM_READ = 0x8CA8, // GL_READ_ONLY
	NW_IM_WRT  = 0x88B9,
	NW_IM_RW   = 0x88BA,
};

/**
 * @brief Enumeration of texture magnification filters.
 */
enum TexMaxFilter {
	NW_LINEAR  = 0x2601, /**< Linear magnification filter. */
	NW_NEAREST = 0x2600	 /**< Nearest magnification filter. */
};

/**
 * @brief Enumeration of texture edge behaviors.
 */
enum TexEdge {
	NW_REPEAT = 0x2901, /**< Repeat texture edge behavior. */
	NW_CLAMP  = 0x812F	/**< Clamp texture edge behavior. */
};

/**
 * @brief Enumeration of supported texture target.
 */
enum TexTarget {
	NW_TEX_2D	 = 0x0DE1, /**< 2D texture.*/
	NW_TEX_2D_MS = 0x9100, /**< Multisample 2D texture, cannot be filtered nor be used to generate mipmaps .*/
};
/**
 * @brief Class representing a texture asset.
 */
class Texture : public Asset {
  public:
	uint32		 _glID = 0;		 /**< The OpenGL ID of the texture. Public for the framebuffer. */
	Vector2<int> _size;			 /**< The size of the texture. */
	bool		 _hasMipMap = 0; /**< Flag indicating whether the texture has mipmaps. */

	Texture() = default;

	void Clean() override;

	void Bind(uint32 slot = 0);

	void BindImageTex(uint32 slot = 0, RWImage access = RWImage::NW_IM_RW);

	void _GPUGen(uint8* data, TexChannelInfo channelInfo);

	void GenMipMap();

	void SetMinFilter(TexMinFilter minFilter);

	void SetMaxFilter(TexMaxFilter maxFilter);

	void SetEdgesBehaviour(TexEdge edge);

	Asset* GetFromCache(void* identifier) override;

	Asset* LoadFromFile(const char* path, TextureIdentifierPtr identifier) override;

	Asset* LoadFromBuffer(void* buffer, void* data) override;

	NW_DECL_RES_LIST(TextureIdentifier, Texture)
};
/**
 * @brief Class representing a mustisample, mostly used internally.
 * @note undocumented
 */
class MSTexture {
  public:
	uint32		 _glID = 0;		  /**< The OpenGL ID of the texture. Public for the framebuffer. */
	Vector2<int> _size;			  /**< The size of the texture. */
	uint16		 _samplesNum = 2; /**< Number of samples. */

	void _GPUGen(TexChannelInfo channelInfo);
	void Bind(bool unbind = 0);
	void Clean();
};


class Texture3D {
    public:
    ui32 _glID;
    bool _hasMipMap = 0;
    int  _fmt = 0;
    v3i  _size;

	Texture3D() = default;
	void Clean();
	void Bind(uint32 slot = 0);
	void BindImageTex(uint32 slot = 0, RWImage access = RWImage::NW_IM_RW);
	void _GPUGen(uint8* data, TexChannelInfo channelInfo, bool _16bitfmt = 0);
	void GenMipMap();
	void SetMinFilter(TexMinFilter minFilter);
	void SetMaxFilter(TexMaxFilter maxFilter);
	void SetEdgesBehaviour(TexEdge edge);
};

