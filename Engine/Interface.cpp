#include <GL/glew.h>
#include <glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "TileMap.h"
#include "Scene.h"
#include "RessourcesLoader.h"


static ImColor bgColor = ImColor(82, 75, 108);
extern double fps;

static bool isActive = true;
static bool sceneViewActive = true;
static bool hierarchyActive = true;
static bool inspectorActive = true;
static int8 selected = -1; //for hiearchy

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



void SceneViewGui() {
	if (sceneViewActive) {
		ImGui::Begin("Scene", 0, ImGuiWindowFlags_MenuBar);
		ImGui::Image((void*)(intptr_t)
			textures["grabTex"].texture,
			ImVec2(RENDERING_WIDTH, RENDERING_HEIGHT), ImVec2(0, 1), ImVec2(1, 0));
		auto temp = ImGui::GetWindowSize();
		RENDERING_HEIGHT = temp.y;
		RENDERING_WIDTH = (int)(temp.y * SCREENRATIO);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Close")) sceneViewActive = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
}

void HierarchyGui() {
	if (hierarchyActive) {
		ImGui::Begin("Hierarchy", &hierarchyActive, ImGuiWindowFlags_MenuBar);
		int8 i = 0;
		for (auto it = Scene::currentScene->sceneObjs.begin(); it < Scene::currentScene->sceneObjs.end(); it++) {
			if (ImGui::Selectable(it->name, selected == i))
				selected = i;
			i++;
		}
		ImGui::End();
	}
	else selected = -1;
}

void InspectorGui() {
	if (inspectorActive) {
		ImGui::Begin("Inspector", &inspectorActive, ImGuiWindowFlags_MenuBar);
		if (selected >= 0 && selected < Scene::currentScene->sceneObjs.size()) {
			ImGui::LabelText(Scene::currentScene->sceneObjs[selected].name, "");
			ImGui::DragInt2("position", &Scene::currentScene->sceneObjs[selected].position.x);
			ImGui::DragFloat2("Scale", &Scene::currentScene->sceneObjs[selected].scale.x);

		}
		ImGui::End();
	}
}
//extern float pitch;
//extern float jumpHeight;
//extern float jumpTime;
void UpdateInferface() {
	ImGui::DockSpaceOverViewport();

	ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);
	ImGui::Text("fps = %f", fps);
	ImGui::ColorEdit3("Background Color", (float*)&bgColor);
	/*ImGui::SliderFloat("pitch", &pitch, 0.0f, 2.0f);
	ImGui::DragFloat("jump height", &jumpHeight);
	ImGui::DragFloat("jump time", &jumpTime);*/


	ImGui::ShowDemoWindow();
	ImGuiIO& io = ImGui::GetIO(); (void)io; //TODO::Fix this
	ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Explore"))
		{
			if (ImGui::MenuItem("Tilemap Editor"))		TileMap::GuiActive = true;
			if (ImGui::MenuItem("Scene Editor"))		Scene::GuiActive = true;
			if (ImGui::MenuItem("Scene View"))			sceneViewActive = true;
			if (ImGui::MenuItem("Hierarchy"))			hierarchyActive = true;
			if (ImGui::MenuItem("Inspector"))			inspectorActive = true;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//Capture mouse
	isMouseOnGui = io.WantCaptureMouse;
	//calling all extern gui functions
	TileMap::Gui();
	Scene::Gui();
	//Calling internal Gui functions


	SceneViewGui();
	HierarchyGui();
	InspectorGui();

	//TODO::Find compromise for intern-extern gui

}
