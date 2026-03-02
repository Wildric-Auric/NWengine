#pragma once
#include "Asset.h"
#include "Globals.h"

#include <string>
#include <unordered_map>

typedef void* TextureIdentifierPtr;

#define NW_RGBA32F  0x8814
#define NW_RGB32F   0x8815
#define NW_RGBA16F  0x881A
#define NW_RGB16F   0x881B
#define NW_RGBA32UI 0x8D70
#define NW_RGB32UI  0x8D71
#define NW_RGBA16UI 0x8D76
#define NW_RGB16UI  0x8D77
#define NW_RGBA8UI  0x8D7C
#define NW_RGB8UI   0x8D7D
#define NW_RGBA32I  0x8D82
#define NW_RGB32I   0x8D83
#define NW_RGBA16I  0x8D88
#define NW_RGB16I   0x8D89
#define NW_RGBA8I   0x8D8E
#define NW_RGB8I    0x8D8F
#define NW_R16F     0x822D
#define NW_R32F     0x822E
#define NW_R32UI    0x8236
enum TexType_Exp {
    TexType_Exp_rgba32f = NW_RGBA32F,
    TexType_Exp_rgba16f = NW_RGBA16F,
    TexType_Exp_r16f    = NW_R16F,
    TexType_Exp_r32f    = NW_R32F,
    TexType_Exp_r32ui   = NW_R32UI,
    TexType_Exp_rgba32ui= NW_RGBA32UI,
};

/**
 * @brief Struct representing the identifier of a texture.
 */
struct TextureIdentifier {
	std::string name;  /**< The name of the texture. */
	uint8		alpha; /**< The alpha value of the texture. */
    TexType_Exp type = TexType_Exp::TexType_Exp_rgba16f;

	/**
	 * @brief Overloaded equality operator for comparing TextureIdentifier objects.
	 * @param other The other TextureIdentifier object to compare with.
	 * @return True if the TextureIdentifier objects are equal, false otherwise.
	 */
	bool operator==(const TextureIdentifier& other) const { return other.type == type && alpha == other.alpha && name == other.name; }
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

//#define GL_READ_ONLY 
//#define GL_WRITE_ONLY 0x88B9
//#define GL_READ_WRITE 0x88BA
enum RWImage {
		NW_IM_READ = 0x88B8, // GL_READ_ONLY
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
    TexType_Exp  type;

	Texture() = default;

	void Clean() override;

	void Bind(uint32 slot = 0);

	void BindImageTex(uint32 slot = 0, RWImage access = RWImage::NW_IM_RW);

	void _GPUGen(uint8* pixelBuffer, TexChannelInfo info, TexType_Exp atype, int extFmt = 0, int compType = 0x1401);

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

