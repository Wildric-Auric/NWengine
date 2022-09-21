#pragma once
#include "NWGui.h"
#include "SceneEditorGui.h"
#include "Scene.h"
#include "Utilities.h"
#include "PostProcessing.h"
#include "Console.h"
#include "ScriptManager.h"
#include "ParticleSystem.h"
#include "imgui\\implot\\implot.h"
class DebugGui {
public:
	static bool isActive;
	static void Show() {
		if (!isActive) return;
		ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);
		ImGui::Text("fps = %f", Globals::fps);
		NWGui::DragValue<int>("Cam pos", &SceneEditorGui::cam.position.x, ImGuiDataType_S32, 2);
		NWGui::DragValue<float>("Zoom", &SceneEditorGui::cam.zoom, ImGuiDataType_Float, 1, 0.1f, 0.0f, 10.0f);
		//DragFloat("Zoom", &SceneEditorGui::cam.zoom, 0.1, 0.0, 10.0);
		static ParticleSystem* ps = nullptr;
		static bool b = 0;
		static int a = 0;
		static bool c = 0;
		static Script* ee = nullptr;
		static GameObject obj = GameObject();
		NWGui::DragValue<int>("Flag", &a, ImGuiDataType_S32, 1, 0.2f, 0, 4);
		ImGui::Checkbox("Save Scene", &b);
		if (ImGui::Checkbox("SCRIPT TEXT", &c)) {
			if (c) {
				c = 0;
				ps = Scene::currentScene->sceneObjs.back().AddComponent<ParticleSystem>();
				ps->prop.lifetime = 10.0f;	
				ps->prop.sDirection = fVec2(0.0, 1.0);

				ps->prop.sScale = fVec2(0.0, 0.0);
				ps->prop.eScale = fVec2(0.3, 0.3);
				ps->prop.scaleVarDuration = 3.0f;
				ps->prop.sDirection = fVec2(1.0f, 0.0f);
				ps->prop.eDirection = fVec2(0.0f, 1.0f);
				ps->prop.directionVarDuration = 3;
				ps->emissionFrequency = 0.1;
				ps->prop.lifetime = 1000;
				ps->prop.lifedistance = 2000;
			}
		};

		if (ps != nullptr) {
			ps->Update();
		}

		GameObject* go = nullptr;

		//------------------------

		//PostProcessing test
		if (b) {
			b = 0;
			for (auto go = Scene::currentScene->sceneObjs.begin(); go!= Scene::currentScene->sceneObjs.end(); go++) {
				if ( go->name == std::string("Warrior")) {
					PostProcessing* pp = go->AddComponent<PostProcessing>();
					pp->SetUp(go->GetComponent<Camera>()->size);
					break;
				}
			}
		}
		//testing isRendered


		//-------------------------

		ImGui::Separator();
		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();
		static double x = 0;
		static double y = 0;
		if (ImPlot::BeginPlot("Hello World")) {
			ImPlot::DragPoint(1504, &x, &y, ImVec4(1.0f,0.0f,0.0f, 1.0f));
			ImPlot::EndPlot();
		}
		static ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::End();
	}
};
