#include <GL/glew.h>
#include <glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "TileMap.h"
#include "Scene.h"
#include "RessourcesLoader.h"


static ImColor bgColor = ImColor(82, 75, 108);
static bool isActive = true;
extern double fps;

void InitInterface(int window) {
	//Create ImGui context
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	ImGui_ImplOpenGL3_Init();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.0f);

}


void UpdateInferface() {
	ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);
	ImGui::Text("fps = %f", fps);
	ImGui::ColorEdit3("Background Color", (float*)&bgColor);
	ImGui::ShowDemoWindow();

	ImGuiIO& io = ImGui::GetIO(); (void)io; //TODO::Fix this

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Tilemap Editor")) { TileMap::GuiActive = true; }
			if (ImGui::MenuItem("Scene Editor")) { Scene::GuiActive = true; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

	}

	//calling all extern gui functions
	TileMap::Gui();
	Scene::Gui();

	//Capture mouse
	isMouseOnGui = io.WantCaptureMouse;

	ImGui::End();

	ImGui::Begin("Scene");
	ImGui::Image((void*)(intptr_t)
		textures["grabTex"].texture,
		ImVec2(RENDERING_WIDTH, RENDERING_HEIGHT), ImVec2(0, 1), ImVec2(1, 0));
	auto temp = ImGui::GetWindowSize();
	RENDERING_HEIGHT = temp.y;
	RENDERING_WIDTH = (int)(temp.y * SCREENRATIO);


	ImGui::End();
}
