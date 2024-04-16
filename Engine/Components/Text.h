#pragma once
#include "Font.h"
#include "GameObject.h"
#include "Shader.h"
#include <list>

/**
 * @brief The Text class represents a text component that can be attached to a GameObject.
 */
class Text : public GameComponent {
public:
    /**
     * @brief GetType returns the type of the Text component.
     * @return The type of the Text component.
     */
    static std::string GetType() { return "Text"; }

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
     * @brief attachedObj is a pointer to the GameObject that the Text component is attached to.
     */
    GameObject* attachedObj = nullptr;

    /**
     * @brief Update is called every frame to update the Text component.
     */
    void Update() override;

    /**
     * @brief GetGameObject returns the GameObject that the Text component is attached to.
     * @return The GameObject that the Text component is attached to.
     */
    void* GetGameObject() override;

    /**
     * @brief SetGameObject sets the GameObject that the Text component is attached to.
     * @param go The GameObject to attach the Text component to.
     */
    void SetGameObject(void* go) override;

    /**
     * @brief UpdateGlyphs updates the glyphs of the Text component.
     */
    void UpdateGlyphs();

    /**
     * @brief SetFont sets the font of the Text component.
     * @param path The path to the font file.
     * @param shader The shader to use for rendering the text.
     */
    void SetFont(const std::string& path, Shader* shader);

    /**
     * @brief SetShader sets the shader of the Text component.
     * @param shader The shader to use for rendering the text.
     */
    void SetShader(Shader* shader);

    /**
     * @brief Gets total size in pixels sets the shader of the text 
     * @return vector of size in pixels.
     * @note Only horizontal size is tested.
     */
    fVec2 GetSize();

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

    /**
     * @brief text is the string of text to be rendered.
     */
    std::string text = "";

    /**
     * @brief isBatched indicates whether the text is batched for rendering.
     */
    bool isBatched = true;

    /**
     * @brief layerOrder indicates the layer order of each glyph, changing this requires calling UpdateGlyph()
     */
    int layerOrder = 0;

    /**
     * @brief colors is the color of the text.
     */
    Vector4<float> colors = Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f); // Each color is 10 bits

    /**
     * @brief position is the position of the text.
     */
    fVec2 position = fVec2(0.0f, 0.0f);

    /**
     * @brief scale is the scale of the text.
     */
    fVec2 scale = fVec2(1.0f, 1.0f);

    /**
     * @brief Serialize serializes the Text component data.
     * @param data The file stream to write the serialized data to.
     * @param offset The offset in the file stream to start writing the serialized data.
     * @return The offset in the file stream after writing the serialized data.
     */
    int Serialize(std::fstream* data, int offset) override;

    /**
     * @brief Deserialize deserializes the Text component data.
     * @param data The file stream to read the serialized data from.
     * @param offset The offset in the file stream to start reading the serialized data.
     * @return The offset in the file stream after reading the serialized data.
     */
    int Deserialize(std::fstream* data, int offset) override;
};