#pragma once
#include "GameObject.h"
#include "Script.h"
#include "Maths.h"
#include "Camera.h"
#include "Shader.h"

/**
 * @brief The Renderer class is responsible for rendering GameObjects using a specified shader.
 */
class Renderer {
public:
    /**
     * @brief Default constructor for the Renderer class.
     */
    Renderer() = default;

    /**
     * @brief Constructor for the Renderer class that takes a shader path as input.
     * @param shaderPath The path to the shader file.
     */
    Renderer(const std::string& shaderPath);

    /**
     * @brief Destructor for the Renderer class.
     */
    ~Renderer();

    /**
     * @brief The container for the GameObject components.
     */
    GameObject componentContainer = GameObject();

    /**
     * @brief The target camera for rendering.
     */
    Camera* target = nullptr;

    /**
     * @brief The stretch coefficients for rendering.
     */
    fVec2 stretchCoeff = fVec2(1.0f, 1.0f);

    fVec2 _offscreenCoeff = fVec2(1.0f, 1.0f);

    TexMinFilter _minFilter = TexMinFilter::NW_MIN_LINEAR; 

    TexMaxFilter _magFilter = TexMaxFilter::NW_LINEAR; 

    void SetOffScreenSizeMultiplier(const fVec2&);

    void SetOnScreenSizeMultiplier(const fVec2&);

    void SetTexParams(const TexMinFilter, const TexMaxFilter);

    bool _DrawPrep();

    /**
     * @brief Operator overload for adding a renderer as a decorator.
     * @param renderer The renderer to add as a decorator.
     * @param captureOnDefaultFrame Boolean indicating if the renderer should capture on the default framebuffer.
     * @return The modified renderer.
     */
    Renderer* operator()(Renderer* renderer, bool captureOnDefaultFrame = 0);

    /**
     * @brief Operator overload for adding a renderer as a decorator.
     * @param captureOnDefaultFrame Boolean indicating if the renderer should capture on the default framebuffer.
     * @return The modified renderer.
     */
    Renderer* operator()(bool captureOnDefaultFrame = 0);

    /**
     * @brief Draws on the default framebuffer.
     */
    void DrawOnDefaultFrame();

    /**
     * @brief Captures on the camera framebuffer.
     */
    void CaptureOnCamFrame();

    /**
     * @brief Sets this renderer as the current renderer.
     */
    void Use();

    /**
     * @brief Sets the default renderer as the current renderer.
     *        The default renderer is managed by the Core frontend or a custom one.
     */
    void Unuse();

    /**
     * @brief Sets the shader for this renderer.
     * @param shaderPath The path to the shader file.
     */
    void SetShader(const std::string& shaderPath);

    void SetShader(const ShaderText& st, ShaderIdentifier* id);

    /**
     * @brief Adds components to the componentContainer.
     */
    void SetUp();

    void Clean();

    Camera* GetCamera();
    

    /**
     * @brief Initializes the default renderer.
     */
    static void Init();

    /**
     * @brief Destroys the default renderer.
     */
    static void Destroy();

    /**
     * @brief The default renderer.
     */
    static Renderer* defaultRenderer;

    /**
     * @brief The current renderer.
     */
    static Renderer* currentRenderer;

    /**
     * @brief Gets the type of the Renderer class.
     * @return The type of the Renderer class.
     */
    inline static std::string GetType() { return "Renderer"; }
};
