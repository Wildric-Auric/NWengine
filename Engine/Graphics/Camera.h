#pragma once
#include "GameObject.h"
#include "FrameBuffer.h"
#include "Maths.h"

/**
 * @brief The Camera class represents a camera in the game world.
 */
class Camera : public GameComponent {
public:
    /**
     * @brief Gets the type of the camera.
     * @return The type of the camera.
     */
    static std::string GetType() { return "Camera"; };

    /**
     * @brief Updates the active camera.
     *
     * This static method calls capture on ActiveCamera if not null.
     */
    static void UpdateActiveCamera();

    Matrix4<float> projectionMatrix; /**< The projection matrix of the camera. */
    Matrix4<float> viewMatrix = Matrix4<float>(1.0f); /**< The view matrix of the camera. */

    fVec2 size; /**< The size of the camera. */
    fVec2 viewPortSize; /**< The viewport size of the camera. */
    fVec3 clearColor = fVec3(0.0,0.0, 0.0); /**< The clear color of the camera. */
    float alpha = 1.0f; /**< The alpha value of the camera. */
    bool isActive = 0; /**< Indicates whether the camera is active or not. */
    std::unordered_map<int, fVec4> clearCols;
    MSAAValue _msaa = NW_MSx1;
    
    void _ClearAtts();
    /**
     * @brief Default constructor for the Camera class.
     */
    Camera() = default;

    /**
     * @brief Constructor for the
 Camera class.
     * @param go The game object to attach the camera to.
     */
    Camera(GameObject* go);

    /**
     * @brief Destructor for the Camera class.
     */
    ~Camera();

    /**
     * @brief Captures the camera.
     */
    void Capture();

    /**
     * @brief Updates the camera.
     */
    void Update() override;

    /**
     * @brief Changes the orthographic size of the camera.
     * @param sizeX The new size along the X-axis.
     * @param sizeY The new size along the Y-axis.
     */
    void ChangeOrtho(float sizeX, float sizeY);

    /**
     * @brief Changes the orthographic size and multisample value of the camera.
     * @param sizeX The new size along the X-axis.
     * @param sizeY The new size along the Y-axis.
     * @param msaa The multisample value.
     */
    void ChangeOrthoWithMSAA(float sizeX, float sizeY, MSAAValue msaa);

    /**
     * @brief Sets the general clear value.
     * @param color RGBA color values.
     */
    void SetClearColor(const fVec4& color);

    /**
     * @brief Sets the clear value for a specific attachment.
     * @param i The index of the attachment.
     * @param color RGBA color values.
     */
    void SetClearColor(int i, const fVec4& color);

    fVec2 GetPosition();
    fVec2 GetSize();

    /**
     * @brief Reset specific clear colors.
     */
    void ResetClearColors();

    /**
     * @brief Moves the camera to the target position with interpolation.
     * @param target The target position to move the camera to.
     * @param interpolationTime The time taken for the interpolation.
     */
    void MoveTo(Vector2<int> target, float interpolationTime);

    /**
     * @brief Uses the camera.
     */
    void Use();

    fVec2 position = fVec2(0.0f,0.0f); /**< The position of the camera. */
    float rotation = 0.0f; /**< The rotation of the camera. */
    float zoom = 1.0; /**< The zoom level of the camera. */

    FrameBuffer fbo; /**< The frame buffer object of the camera. */

    static Camera* GetActiveCamera();

    static Camera* ActiveCamera; /**< The active camera. */
};
