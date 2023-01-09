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
		AudioEmitter* ae = nullptr;
		AudioListener* al = nullptr;
		

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
			animator  =  go->GetComponent<Animator>();
		    ae        = go->GetComponent<AudioEmitter>();
			al		  = go->GetComponent<AudioListener>();
			

			ps  = go->GetComponent<ParticleSystem>();

			int n = 1;
			for (std::map<std::string, GameComponent*>::iterator it = go->components.begin(); it != go->components.end();) {
				std::string    key = it->first;
				GameComponent* gc  = it->second;
				it++;
				if (!ImGui::CollapsingHeader(key.c_str())) continue;
				gc->Gui();
				if (!ImGui::Button((std::string("Delete##") + std::to_string(n)).c_str())) continue;
				go->DeleteComponent(key);
				n++;
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
				if (ImGui::Selectable("AudioEmitter") && ae == nullptr)         go->AddComponent<AudioEmitter>();
				if (ImGui::Selectable("AudioListenner") && al == nullptr)       go->AddComponent<AudioListener>();


 				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
};