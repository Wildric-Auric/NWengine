#pragma once
#include"Globals.h"
#include"Asset.h"

#include<unordered_map>

typedef void* TextureIdentifierPtr;
//Input may be given by user in constructor
struct TextureIdentifier {
	std::string name;
	uint8		alpha;

	bool operator==(const TextureIdentifier& other) const {
		return alpha == other.alpha && name == other.name;
	}
};


template <>
struct std::hash<TextureIdentifier>
{
	std::size_t operator()( const TextureIdentifier& t) const
	{
		std::size_t res = 0;
		hashCombine(res, t.name);
		hashCombine(res, t.alpha);
		return res;
	}
};

//GL values of GL_RED, GL_RGB and GL_RGBA
enum TexChannelInfo{
	NW_R    = 0x1903,
	NW_RGB  = 0x1907,
	NW_RGBA = 0x1908

};

enum TexMinFilter {
	NW_MIN_LINEAR                  = 0x2601,
	NW_MIN_NEAREST                 = 0x2600,
	NW_NEAREST_MIPMAP_NEAREST      = 0x2700,
	NW_NEAREST_MIPMAP_LINEAR       = 0x2702
};


enum TexMaxFilter {
	NW_LINEAR                     = 0x2601,
	NW_NEAREST                    = 0x2600
};

enum TexEdge {
	NW_REPEAT = 0x2901,
	NW_CLAMP  = 0x2900
};


class Texture : public Asset {
public:
	uint32		 _glID = 0;  //Public for the framebuffer
	Vector2<int> _size;
	bool         _hasMipMap;

	Texture() = default;
	void Clean()				override;
	void Bind(uint32 slot = 0);

	void _GPUGen(uint8*,TexChannelInfo);
	void GenMipMap();
	void SetMinFilter(TexMinFilter);
	void SetMaxFilter(TexMaxFilter);
	void SetEdgesBehaviour(TexEdge);


	Asset*    GetFromCache(void* identifier)						       		override;
	Asset*    LoadFromFile(const char* path, TextureIdentifierPtr identifier)	override;
	Asset*    LoadFromBuffer(void* buffer, void* data)						    override;

	NW_DECL_RES_LIST(TextureIdentifier, Texture)
};
