#pragma once
#include "Globals.h"

struct OpenGLInfo {
	unsigned char minVersion = 3;
	unsigned char maxVersion = 3;
	// bool  profileCore = 1;
	bool disableCompatibility = 1;
};

enum NWStencilBehaviour {
	NW_LESS		  = 0x0201,
	NW_EQUAL	  = 0x0202,
	NW_LEQUAL	  = 0x0203,
	NW_GL_GREATER = 0x0204,
	NW_NOTEQUAL	  = 0x0205,
	NW_GEQUAL	  = 0x0206,
	NW_ALWAYS	  = 0x0207
};

enum NWMemoryBarrierBit {
	VERTEX_ATTRIB_ARRAY_BARRIER_BIT = 0x00000001,
	ELEMENT_ARRAY_BARRIER_BIT		= 0x00000002,
	UNIFORM_BARRIER_BIT				= 0x00000004,
	TEXTURE_FETCH_BARRIER_BIT		= 0x00000008,
	SHADER_IMAGE_ACCESS_BARRIER_BIT = 0x00000020,
	COMMAND_BARRIER_BIT				= 0x00000040,
	PIXEL_BUFFER_BARRIER_BIT		= 0x00000080,
	TEXTURE_UPDATE_BARRIER_BIT		= 0x00000100,
	BUFFER_UPDATE_BARRIER_BIT		= 0x00000200,
	FRAMEBUFFER_BARRIER_BIT			= 0x00000400,
    CLIENT_MAPPED_BUFFER_BARRIER_BIT = 0x00004000
};

/**
 * @brief The Context class represents the application context.
 */
class Context {
  public:
	/**
	 * @brief The window handle. Should be converted to NWin::Window* to be used
	 */
	static void* window;

	static void GetWinDrawAreaSize(fVec2*);
	static void GetWinSize(fVec2*);

	/**
	 * @brief Initializes the application context.
	 * @param scrWidth The screen width.
	 * @param scrHeight The screen height.
	 * @return A pointer to the initialized context.
	 */
	static void* InitContext(int scrWidth, int scrHeight);

	/**
	 * @brief Checks if the application should close.
	 * @return True if the application should close, false otherwise.
	 */
	static bool ShouldClose();

	/**
	 * @brief Updates the application context.
	 */
	static void Update();

	/**
	 * @brief Enables or disables blending.
	 * @param status The blending status. True to enable blending, false to disable blending.
	 */
	static void EnableBlend(bool status = true);

	/**
	 * @brief Enables or disables vsync.
	 * @param status vsync status. 1 to enable vsync, 0 to disable.
	 */
	static void EnableVSync(int status = 1);

	/**
	 * @brief Enables or disables wireframe rendering.
	 * @param status The wireframe status. True to enable wireframe rendering, false to disable wireframe rendering.
	 */
	static void EnableWireframe(bool status = true);

	/**
	 * @brief Enables or disables depth testing.
	 * @param status The depth testing status. True to enable depth testing, false to disable depth testing.
	 */
	static void EnableDepthTest(bool status = true);

	static void EnableStencilTest(bool status = true);

	static void SetStencilWrite(bool val);

	static void SetStencilFunc(NWStencilBehaviour b);

	static void SetStencilMask(bool val);

	/**
	 * @brief Clears the screen with the specified color.
	 * @param r The red component of the color.
	 * @param g The green component of the color.
	 * @param b The blue component of the color.
	 * @param a The alpha component of the color.
	 */
	static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	static void Clear(const fVec4&);
	static void ClearStencilBuff();
	static void ClearColorBuff(const fVec4&);
	static void ClearDepthBuff();

	/**
	 * @brief Sets the viewport.
	 * @param x The x-coordinate of the viewport.
	 * @param y The y-coordinate of the viewport.
	 * @param sizeX The width of the viewport.
	 * @param sizeY The height of the viewport.
	 */
	static void SetViewPort(int x, int y, int sizeX, int sizeY);

	/**
	 * @brief Sets the application to fullscreen mode or windowed mode.
	 * @param state The fullscreen state. True for fullscreen mode, false for windowed mode.
	 */
	static void SetFullscreen(bool state = true);

	/**
	 * @brief Sets the title of the application window.
	 * @param title The title of the window.
	 */
	static void SetTitle(const char* title);

	static void NWMemoryBarrier(int);

	/**
	 * @brief Destroys the application context.
	 */
	static void Destroy();

	/**
	 * @brief The vertical synchronization value.
	 */
	static int _vSync;

	/**
	 * @brief The width of the application window.
	 */
	static int WINDOW_WIDTH;

	/**
	 * @brief The height of the application window.
	 */
	static int WINDOW_HEIGHT;
	/**
	 *@brief change to set OpenGL context creation parameters such as minor/major versions.
	 */
	static OpenGLInfo _glInfo;
};


namespace GPUCap {
    struct ComputeShaderCapabilities {
    	iVec3 workGroupNum;
    	iVec3 localSize;
    	int32 maxInvoc;
    };

    int QueryMaxTexture();
    bool QueryComputeShaderCap(ComputeShaderCapabilities*);
}
