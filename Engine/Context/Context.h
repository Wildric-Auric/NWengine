#pragma once

class Context {
public:
	static void* window;  //Should be converted to GLFWwindow* to be used
	static void* InitContext(int scrWidth, int scrHeight);
	static bool ShouldClose();
	static void Update();
	static void EnableBlend(bool status = 1);
	static void EnableWireframe(bool status = 1);
	static void EnableDepthTest(bool status = 1);
	static void Clear(float r = 0.0f, float g = .0f, float b = 0.0f, float a = 1.0f);
	static void SetViewPort(int x, int y, int sizeX, int sizeY);
	static void SetFullscreen(bool state = 1);
	static void SetTitle(const char* title);
	static void Destroy();
	static int  dllFlag;
	static int vSync;
	static int  NATIVE_WIDTH;
	static int NATIVE_HEIGHT;

	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
};

