#pragma once
#include "NWGui.h"
#include "HierarchyGui.h"
#include "ScriptManager.h"
#include "Utilities.h"
#include "Animation.h"
#include "Animator.h"

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
		ParticleSystem* ps = nullptr;
		Animator* animator = nullptr;
		

		if (HierarchyGui::selected > -1 && HierarchyGui::selected < Scene::currentScene->sceneObjs.size()) {
			auto it = Scene::currentScene->sceneObjs.begin();
			std::advance(it, HierarchyGui::selected);
			go = &(*it);
			//TODO::Check performance of getting all components each frame

			sprite    = go->GetComponent<Sprite>();
			transform = go->GetComponent<Transform>();
			collider  = go->GetComponent<Collider2>();
			script    = go->GetComponent<Script>();
			cam       = go->GetComponent<Camera>();
			animator =  go->GetComponent<Animator>();
			

			ps  = go->GetComponent<ParticleSystem>();
			if (transform != nullptr) {
				if (ImGui::CollapsingHeader("Transform")) {
					static iVec2 guiPosition;
					guiPosition = transform->position;
					if (NWGui::DragValue<int>("Position", &guiPosition.x, ImGuiDataType_S32, 2)) {
						transform->position = guiPosition;
					};
					ImGui::Separator();
					NWGui::DragValue<float>("Scale", &transform->scale.x, ImGuiDataType_Float, 2, 0.01f);
					ImGui::Separator();
					if (ImGui::Button("Delete##1")) go->DeleteComponent<Transform>();
				}
			}

			if (sprite != nullptr) {
				if (ImGui::CollapsingHeader("Sprite")) {
					if (NWGui::DragValue<uint32>("Layering Order", &sprite->sortingLayer, ImGuiDataType_U32, 1))
						sprite->SetSortingLayer(sprite->sortingLayer);
					ImGui::Separator();
					if (NWGui::FileHolder("Texture", sprite->texture->name)) {
						std::string path = GetFile("Image Files\0*.png;*.jpeg;*.jpg\0*.*\0");
						if (path != "") sprite->SetTexture(path);
					}
					ImGui::Separator();
					if (NWGui::FileHolder("Shader", sprite->shader->name.c_str())) {
						std::string path = GetFile("Shader Files\0*.shader\0*.*\0");
						if (path != "") sprite->SetShader(path);
					}
					ImGui::Separator();
					if (ImGui::Button("Delete##2")) go->DeleteComponent<Sprite>();
				}
			}

			if (script != nullptr) {
				if (ImGui::CollapsingHeader("Script")) {
					std::string text = "None";
					if (script->script != nullptr) text = script->script->GetName();
					if (NWGui::FileHolder("Script", text.c_str())) {
						std::string path = GetFile("Text Files\0*.h\0*.*\0");
						std::string filename = "";
						GetFileName(path, &filename);
						if (path != "") script->script = ScriptManager::CreateScript(filename, go); //TODO::Get if file is valid
					}
					if (ImGui::Button("Delete##3")) go->DeleteComponent<Script>();
				}
			}

			if (collider != nullptr) {
				if (ImGui::CollapsingHeader("Collider")) {
					if (ImGui::Button("Delete##4")) go->DeleteComponent<Collider2>();
				}
			}

			using namespace std::string_literals;
			if (cam != nullptr) {
				if (ImGui::CollapsingHeader("Camera")) {
					//TODO::Handle no camera in the scene
				
					static std::string array = (cam->ActiveCamera->attachedObj->name + "\0"s);
					static int arraySize = 1;
					static int camIndex = 0;

					array = "";
					uint16 count = 0;
					//arraySize = cam->componentList.size();
					//for (auto it = cam->componentList.begin(); it != cam->componentList.end(); it++) {
					//	array += it->first->name + "\0"s;
					//	if (count == camIndex) cam->ActiveCamera = &it->second;
					//	count += 1;
					//}

					if (ImGui::Combo("Active Camera", &camIndex, array.c_str(), arraySize)) {
						//TODO::Only update array if user interacts with combo
					};

					NWGui::DragValue<int>("Camera Position", &(cam->position.x), ImGuiDataType_S32, 2);
					if (NWGui::DragValue<int>("Camera Size", &(cam->size.x), ImGuiDataType_S32, 2)) {
						cam->ChangeOrtho(cam->size.x, cam->size.y);
						cam->fbo = FrameBuffer(cam->size.x, cam->size.y); //TODO:: NOT DO THIS HERE; just testing
						//cam->viewPortSize.x = cam->size.x;
						//cam->viewPortSize.y = cam->size.y;
					}

					//if (ImGui::DragInt2("Viewport", &(cam->viewPortSize.x))) {
					//	cam->fbo = FrameBuffer(cam->viewPortSize.x, cam->viewPortSize.y); //TODO:: NOT DO THIS HERE; just testing
					//};


					if (ImGui::Button("Delete##5")) go->DeleteComponent<Camera>(); //TODO::ACTIVE CAMERA ERROR!!
				}
			}

			if (ps != nullptr && ImGui::CollapsingHeader("Particle System")) {
				if (ImGui::TreeNode("Particles properties")) {
					NWGui::DragValue<float>("Lifetime", &ps->prop.lifetime, ImGuiDataType_Float);
					GUI_SEP;
					NWGui::DragValue<int>("Lifedistance", &ps->prop.lifedistance, ImGuiDataType_S32);
					GUI_SEP;
					NWGui::DragValue<float>("Relative start position", &ps->prop.sPosition, ImGuiDataType_Float, 2);
					GUI_SEP;
					NWGui::DragValue<float>("Starting direction", &ps->prop.sDirection.x, ImGuiDataType_Float, 2, 0.05f, -1.0f, 1.0f);
					GUI_SEP;
					NWGui::DragValue<float>("End direction", &ps->prop.eDirection.x, ImGuiDataType_Float, 2, 0.05f, -1.0f, 1.0f);
					GUI_SEP;
					NWGui::DragValue<float>("Direction variation duration", &ps->prop.directionVarDuration, ImGuiDataType_Float);
					GUI_SEP;

					NWGui::DragValue<float>("Starting scale", &ps->prop.sScale.x, ImGuiDataType_Float, 2);
					GUI_SEP;
					NWGui::DragValue<float>("End scale", &ps->prop.eScale.x, ImGuiDataType_Float, 2);
					GUI_SEP;
					NWGui::DragValue<float>("Scale variation duration", &ps->prop.scaleVarDuration, ImGuiDataType_Float);
					GUI_SEP;

					NWGui::DragValue<float>("Starting speed", &ps->prop.sSpeed, ImGuiDataType_Float);
					GUI_SEP;
					NWGui::DragValue<float>("End speed", &ps->prop.eSpeed, ImGuiDataType_Float);
					GUI_SEP;
					NWGui::DragValue<float>("speed variation duration", &ps->prop.speedVarDuration, ImGuiDataType_Float);
					GUI_SEP; GUI_NEWLINE;
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Emission properties")) {
					NWGui::DragValue<double>("Emission frequency", &ps->emissionFrequency, ImGuiDataType_Double);
					GUI_SEP;
					NWGui::DragValue<uint16>("Emission quantity", &ps->emissionQuantity, ImGuiDataType_U16);
					GUI_SEP;
					NWGui::CheckBox("Recycle particles", &ps->recycle);
					GUI_SEP;
					NWGui::DragValue<int>("Max particles(experimental)", &ps->maxParticles, ImGuiDataType_S32);
					ImGui::TreePop();
				}	
			}
			
			if (animator != nullptr && ImGui::CollapsingHeader("Animator")) {
				if (ImGui::TreeNode("animation")) {};
			}

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
				if (ImGui::Selectable("Particle system") && ps == nullptr)      go->AddComponent<ParticleSystem>();
 				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
};