#pragma once
#include "ComponentTypes.h"
#include "FrameBuffer.h"
#include "GameObject.h"
#include "Image.h"
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

	NW_ST_GET_TYPE_IMPL(Camera);
	/**
	 * @brief Updates the active camera.
	 *
	 * This static method calls capture on ActiveCamera if not null.
	 */
	static void UpdateActiveCamera();

	Matrix4<float> projectionMatrix;				  /**< The projection matrix of the camera. */
	Matrix4<float> viewMatrix = Matrix4<float>(1.0f); /**< The view matrix of the camera. */

	fVec2						   size;							  /**< The size of the camera. */
	fVec2						   viewPortSize;					  /**< The viewport size of the camera. */
	fVec3						   clearColor = fVec3(0.0, 0.0, 0.0); /**< The clear color of the camera. */
	float						   alpha	  = 1.0f;				  /**< The alpha value of the camera. */
	std::unordered_map<int, fVec4> clearCols;
	bool						   _wireframe = 0;

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

	Camera* BeginCap(bool dontClear = 0);
	void	EndCap(Camera* oldCam);

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
	void ChangeOrthoFull(float sizeX, float sizeY, MSAAValue msaa, TexType_Exp type);

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

	void EnableWireframeRendering();
	void DisableWireframeRendering();

	fVec2 GetPosition();
	fVec2 GetSize();
    MSAAValue GetMSAAValue();
    TexType_Exp GetTexType();

	FrameBuffer* GetFbo();
	/**
	 * @brief Reset specific clear colors.
	 */
	void ResetClearColors();

	void MoveTo(Vector2<int> target, float interpolationTime);

	void Use();

	fVec2 position = fVec2(0.0f, 0.0f); /**< The position of the camera. */
	float rotation = 0.0f;				/**< The rotation of the camera. */
	float zoom	   = 1.0;				/**< The zoom level of the camera. */

    TexType_Exp  type = TexType_Exp_rgba16f; 
	FrameBuffer	 fbo;  /**< The frame buffer object of the camera. */
	FrameBuffer* lfbo; // last bound before capture

	static Camera* GetActiveCamera();
	static Camera* ActiveCamera;
};
