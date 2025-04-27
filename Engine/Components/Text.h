#pragma once
#include "Font.h"
#include "GameObject.h"
#include "Shader.h"
#include "CoordSys.h"
#include <list>
#include "ComponentTypes.h"
#include "Utilities.h"

enum class TextHorizontalAlignment {
    LEFT   = 0,
    RIGHT  = 1,
    CENTER = 1
};

struct TextConstraint {
    float boxHorizontalWrap = INFINITY;
    float fixedLineSpacing = 0.0f; 
    TextHorizontalAlignment halign  = TextHorizontalAlignment::LEFT;
};

struct TextIterData {
    int chrNum   = 0;
    int chrIndex = 0;
    void* other = 0;
};

struct TextConstraintIterData {
    fVec2 cur;
    int lineNum = 1;
    int lastBearing = 0;
};


typedef void (*CharacterUpdateCallback)(Character*, TextIterData*);

/**
 * @brief The Text class represents a text component that can be attached to a GameObject.
 */
class Text : public GameComponent {
public: 
    /**
     * @brief GetType returns the type of the Text component.
     * @return The type of the Text component.
     */
    NW_ST_GET_TYPE_IMPL(Text);

    /**
     * @brief Text constructor.
     */
    Text() = default;

    /**
     * @brief Text destructor.
     */
    ~Text();

    /**
     * @brief Text constructor with GameObject parameter.
     * @param go The GameObject to attach the Text component to.
     */
    Text(GameObject* go);

    /**
     * @brief Update is called every frame to update the Text component.
     */
    void Update() override;

    /**
     * @brief UpdateGlyphs updates the glyphs of the Text component.
     */
    void UpdateGlyphs(bool dontRender = 0);

    void ApplyConstraint(Character*, TextConstraintIterData*);
    void SetChrComps(Character* chr, char c);

    void SetPosition(const fVec2&);

    void SetScale(const fVec2&);
    
    inline Shader* GetShader() {return _shader;};

    void SetBoxHorizontalWrap(const float);

    void SetHorizontalAlignment(const TextHorizontalAlignment);

    void SetFixedLineSpacing(const float); 

    void MapOnChar(CharacterUpdateCallback, void*);

    fVec2 GetPosition();
    fVec2 GetPostionTopLeft();
    /**
     * @brief SetFont sets the font of the Text component.
     * @param path The path to the font file.
     * @param shader The shader to use for rendering the text.
     */
    void SetFont(const FontIdentifier& id, Shader* shader);

    void SetFont(const FontIdentifier& id);

    void SetFont(const FontIdentifier& id, const std::string& shdrPath);

    void SetFont(const FontIdentifier& fid, const ShaderText& st, ShaderIdentifier* id);

    void SetContent(const char*);

    void SetContentAndUpdateGlyphs(const char*);

    void SetConstraint(const TextConstraint&);

    void SetChrCallback(CharacterUpdateCallback);

    /**
     * @brief SetShader sets the shader of the Text component.
     * @param shader The shader to use for rendering the text.
     */
    void SetShader(Shader* shader);

    void SetShader(const ShaderText& st, ShaderIdentifier* id);

    void SetShader(std::string path);

    /**
     * @brief Gets total size in pixels sets the shader of the text 
     * @return vector of size in pixels.
     * @note Only horizontal size is tested.
     */
    fVec2 GetSize();

    float _bearing = 0.0f; 

    NWCoordSys::BoundingBox& GetBBRef();
    void GetBB(NWCoordSys::BoundingBox*);

    void CalcBB(TextConstraintIterData*);
    void SetRelCharPos(Character*);

    void Render();
    void StopRendering();

    void DirectDraw();

    CharacterUpdateCallback chrCbk = [](Character*,TextIterData*)->void {};
    /**
     * @brief characters is a list of characters that make up the text.
     */
    std::list<Character> characters;

    /**
     * @brief _shader is a pointer to the shader used for rendering the text.
     */
    Shader* _shader = nullptr;

    /**
     * @brief font is a pointer to the font used for rendering the text.
     */
    Font* font = nullptr;

    TextConstraint constraints; 
    /**
     * @brief text is the string of text to be rendered.
     */
    std::string text = "";

    /**
     * @brief isBatched indicates whether the text is batched for rendering.
     */
    bool isBatched = true;
    bool _isRendered = true;
    /**
     * @brief layerOrder indicates the layer order of each glyph, changing this requires calling UpdateGlyph()
     */
    int layerOrder = 0;

    /**
     * @brief colors is the color of the text.
     */
    Vector4<float> colors = Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f); // Each color is 10 bits

    /**
     * @brief scale is the scale of the text.
     */
    fVec2 scale = fVec2(1.0f, 1.0f);

    NWCoordSys::BoundingBox _bb;
};
