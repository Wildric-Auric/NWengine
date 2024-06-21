#pragma once

struct OpenGLInfo {
    unsigned char minVersion = 3;
    unsigned char maxVersion = 3;
    //bool  profileCore = 1;
    bool  disableCompatibility = 1;
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
     * @brief Enables or disables wireframe rendering.
     * @param status The wireframe status. True to enable wireframe rendering, false to disable wireframe rendering.
     */
    static void EnableWireframe(bool status = true);

    /**
     * @brief Enables or disables depth testing.
     * @param status The depth testing status. True to enable depth testing, false to disable depth testing.
     */
    static void EnableDepthTest(bool status = true);

    /**
     * @brief Clears the screen with the specified color.
     * @param r The red component of the color.
     * @param g The green component of the color.
     * @param b The blue component of the color.
     * @param a The alpha component of the color.
     */
    static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

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

    /**
     * @brief Destroys the application context.
     */
    static void Destroy();

    /**
     * @brief The vertical synchronization value.
     */
    static int vSync;

    /**
     * @brief The native width of the application window.
     */
    static int NATIVE_WIDTH;

    /**
     * @brief The native height of the application window.
     */
    static int NATIVE_HEIGHT;

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
    static OpenGLInfo  _glInfo;
};
