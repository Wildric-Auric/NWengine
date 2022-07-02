#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "HierarchyGui.h"

class InspectorGui {
public:
	static bool isActive;
	static void Show() {

		if (!isActive) return;

		ImGui::Begin("Inspector", &isActive);

		GameObject* go = nullptr;
		Sprite* sprite = nullptr;
		Transform* transform = nullptr;
		Collider2* collider = nullptr;
		Script* script = nullptr;
		Camera* cam = nullptr;

		if (HierarchyGui::selected > -1 && HierarchyGui::selected < Scene::currentScene->sceneObjs.size()) {
			go = &Scene::currentScene->sceneObjs[HierarchyGui::selected];
			//TODO::Check performance of getting all components each frame
			sprite = go->GetComponent<Sprite>();
			transform = go->GetComponent<Transform>();
			collider = go->GetComponent<Collider2>();
			script = go->GetComponent<Script>();
			cam = go->GetComponent<Camera>();

			if (transform != nullptr) {
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragInt2("Position", &transform->position.x);
					ImGui::DragFloat2("Scale", &transform->scale.x, 0.01f);
					if (ImGui::Button("Delete##1")) go->DeleteComponent<Transform>();
				}
			}

			if (sprite != nullptr) {
				if (ImGui::CollapsingHeader("Sprite")) {
					ImGui::DragScalar("Layering Order", ImGuiDataType_S16, &sprite->sortingLayer);
					ImGui::NewLine();
					if (ImGui::Button("Delete##2")) go->DeleteComponent<Sprite>();
				}
			}

			if (script != nullptr) {
				if (ImGui::CollapsingHeader("Script")) {
					if (ImGui::Button("Delete##3")) go->DeleteComponent<Script>();
				}
			}

			if (collider != nullptr) {
				if (ImGui::CollapsingHeader("Collider")) {
					if (ImGui::Button("Delete##4")) go->DeleteComponent<Collider2>();
				}
			}

			if (cam != nullptr) {
				if (ImGui::CollapsingHeader("Camera")) {
					if (ImGui::Button("Delete##5")) go->DeleteComponent<Camera>(); //TODO::ACTIVE CAMERA ERROR!!
				}
			}

		
			ImGui::Separator();
			ImGui::NewLine();
			ImGui::NewLine();

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("popup0");
			}

			if (ImGui::BeginPopup("popup0")) {
				if (ImGui::Selectable("Transform") && transform == nullptr)		go->AddComponent<Transform>();
				if (ImGui::Selectable("Sprite") && sprite == nullptr)			go->AddComponent<Sprite>();
				if (ImGui::Selectable("Script") && script == nullptr)			go->AddComponent<Script>();
				if (ImGui::Selectable("Collider") && collider == nullptr)		go->AddComponent<Collider2>();
				if (ImGui::Selectable("Camera") && cam == nullptr)				go->AddComponent<Camera>();

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
};