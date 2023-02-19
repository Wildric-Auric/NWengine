#pragma once
#include "NWGui.h"
#include "HierarchyGui.h"
#include "ScriptManager.h"
#include "Utilities.h"
#include "Animation.h"
#include "Animator.h"


#define ADD_COMPONENT_TO_INSPECTOR(type, go) if (ImGui::Selectable(#type)) go->AddComponent<type>();
class InspectorGui {
public:
	static bool isActive;
	static void Show() {

		if (!isActive) return;


		ImGui::Begin("Inspector", &isActive);

		if (Scene::currentScene == nullptr) {
			ImGui::End();
			return;
		}

		

		GameObject* go = nullptr;

		if (HierarchyGui::selected > -1 && HierarchyGui::selected < Scene::currentScene->sceneObjs.size()) {
			auto it = Scene::currentScene->sceneObjs.begin();
			std::advance(it, HierarchyGui::selected);
			go = &(*it);

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
				ADD_COMPONENT_TO_INSPECTOR(Transform,	   go);
				ADD_COMPONENT_TO_INSPECTOR(Sprite   ,	   go);
				ADD_COMPONENT_TO_INSPECTOR(Collider ,	   go);
				ADD_COMPONENT_TO_INSPECTOR(Camera   ,	   go);
				ADD_COMPONENT_TO_INSPECTOR(ParticleSystem, go);
				ADD_COMPONENT_TO_INSPECTOR(AudioEmitter  , go);
				ADD_COMPONENT_TO_INSPECTOR(AudioListener , go);
				ADD_COMPONENT_TO_INSPECTOR(Script,         go)
 				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
};

#undef ADD_COMPONENT_TO_INSPECTOR