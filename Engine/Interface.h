#pragma once

#include "Interface.h"
#include <GL/glew.h>
#include <glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "TileMap.h"
#include "Scene.h"
#include "RessourcesLoader.h"
#include "Utilities.h"
#include <tuple>


extern double fps;

static ImColor bgColor = ImColor(82, 75, 108);
static bool isActive = true;
static bool sceneViewActive = true;
static bool hierarchyActive = true;
static bool inspectorActive = true;
static bool solutionExplorerActive = true;
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

static std::vector<   std::tuple<std::string, int>   > explorerData;

static std::vector<int> accumulation;

//Function Solution explorer code is the least readable or maintainable in this project...
//TODO:: Add refresh button; hint: button triggering "first" variable

static void func(int i) {

	std::string type;
	int childNum = std::get<1>(explorerData[i]);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	if (childNum > 0) {
		type = "Folder";
		bool open = ImGui::TreeNodeEx(std::get<0>(explorerData[i]).c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
		ImGui::TableNextColumn();
		ImGui::Text(type.c_str());
		if (open)
		{
			int count = i;
			for (int j = 0; j < childNum; j++) {
				count += 1;
				func(count);
				count += accumulation[count];
			}
			ImGui::TreePop();
		}

	}
	else {
		ImGui::TreeNodeEx(std::get<0>(explorerData[i]).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
		type = "File";
		ImGui::TableNextColumn();
		ImGui::Text(type.c_str());
	}
}

void SolutionExplorerGui() {
	if (solutionExplorerActive) {
		ImGui::Begin("Solution Explorer", &solutionExplorerActive, ImGuiWindowFlags_MenuBar);

		//Building files list
		static bool first = 1;
		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
		if (first) {
			auto files = GetRecusivelyDirFiles("C:/Users/HP/Desktop/NWengine/Ressources");
			auto childNum = GetRecusivelyFilesNumber("C:/Users/HP/Desktop/NWengine/Ressources");
			std::vector<int> temp;
			accumulation.clear();
			for (int8 i = 0; i < files.size(); i++) {
				int num = childNum[i + 1];
				auto a = std::make_tuple(files[i], num);
				explorerData.push_back(a);
				temp.push_back(num);
				accumulation.push_back(-1);
			}
			auto a = std::make_tuple("Debug", GetDirFiles("C:/Users/HP/Desktop/NWengine/Ressources").size());
			explorerData.insert(explorerData.begin(), a);
			temp.insert(temp.begin(), GetDirFiles("C:/Users/HP/Desktop/NWengine/Ressources").size());
			accumulation.push_back(-1);
			AccumulateChildren(&temp, &accumulation);
			first = false;
		};


		//Building tree view
		if (ImGui::BeginTable("Solution Explorer", 2)) {
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			func(0);
			ImGui::EndTable();
		}

		ImGui::End();
	}

};


//extern float pitch;
//extern float jumpHeight;
//extern float jumpTime;
extern float uniformTest;
void UpdateInferface() {
	ImGui::DockSpaceOverViewport();

	ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);
	ImGui::Text("fps = %f", fps);
	ImGui::ColorEdit3("Background Color", (float*)&bgColor);
	/*ImGui::SliderFloat("pitch", &pitch, 0.0f, 2.0f);
	ImGui::DragFloat("jump height", &jumpHeight);
	*/
	ImGui::DragFloat("Post processing cells", &uniformTest);


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
			if (ImGui::MenuItem("Solution Explorer"))   solutionExplorerActive = true;
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
	SolutionExplorerGui();
	//TODO::Find compromise for intern-extern gui

}
