#pragma once
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "Texture.h"
#include "Asset.h"

typedef void* NW_FT_Lib ;
typedef void* NW_FT_Face;
typedef std::string FontIdentifier;

/**
 * @brief Class representing a glyph.
 */
class Glyph {
public:
    Glyph() = default;

    /*
     * @brief Deletes the glyph.
     */
    void Delete();

    iVec2 ppem;
    iVec2 advance; /**< The advance value of the glyph. */
    iVec2 bearing; /**< The bearing of the glyph. */
    iVec2 size; /**< The size of the glyph. */
    Texture texture; /**< The texture of the glyph. */

    float GetAdvanceX();
    float GetAdvanceY();
    void  GetBearing(fVec2*);
    void  GetSize(fVec2*);
    void  GetButtomLeftPosOffset(fVec2*);
    void  GetCenterPosOffset(fVec2*);
};

/**
 * @brief Class representing a font asset.
 */
class Font : public Asset {
public:
    NW_FT_Face _face = nullptr; /**< The FreeType face. */
    std::unordered_map<char, Glyph> charactersMap; /**< The map of characters to glyphs. */
    uint32 nativeSize = 64;

    Font() = default;

    /**
     * @brief Loads a font from a file.
     * @param path The path to the font file.
     * @return True if the font is loaded successfully, false otherwise.
     */
    bool LoadFont(std::string path);

    void Clean() override;

    Asset* GetFromCache(void* identifier) override;
    Asset* LoadFromFile(const char* path, void* data) override;
    Asset* LoadFromBuffer(void* ftFace, void* data) override;

    static NW_FT_Lib lib; /**< The FreeType library. */

    /**
     * @brief Initializes the FreeType library.
     * @return True if the library is initialized successfully, false otherwise.
     */
    static bool Init();

    /**
     * @brief Destroys the FreeType library.
     */
    static void Destroy();

    NW_DECL_RES_LIST(FontIdentifier, Font);
};

/**
 * @brief Struct representing a character.
 */
struct Character {
    Glyph* glyph; /**< The glyph of the character. */
    GameObject go; /**< The game object associated with the character. */
};
