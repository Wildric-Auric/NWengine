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

    /**
     * @brief Deletes the glyph.
     */
    void Delete();

    uint32 advance = 0; /**< The advance value of the glyph. */
    iVec2 bearing; /**< The bearing of the glyph. */
    iVec2 size; /**< The size of the glyph. */
    Texture texture; /**< The texture of the glyph. */
};

/**
 * @brief Class representing a font asset.
 */
class Font : public Asset {
private:
    NW_FT_Face face = nullptr; /**< The FreeType face. */
public:
    std::unordered_map<char, Glyph> charactersMap; /**< The map of characters to glyphs. */

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
