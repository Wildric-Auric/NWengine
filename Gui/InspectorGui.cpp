#include "imgui/imgui.h"

#include "InspectorGui.h"
#include "HierarchyGui.h"
#include "Scene.h"
#include "Gui.h"
#include "Components.h"
#include "Camera.h"
#include "NWGui.h"

#define ADD_COMPONENT_TO_INSPECTOR(type, go) if (ImGui::Selectable(#type)) go->AddComponent<type>();

void InspectorGui::Init() { this->isActive = 1; }
void InspectorGui::Show() {

	if (!isActive) return;


	ImGui::Begin("Inspector", &isActive);

	if (Scene::currentScene == nullptr) {
		ImGui::End();
		return;
	}



	GameObject* go = nullptr;
	HierarchyGui* hw = (HierarchyGui*)(Gui::Windows.find((uint32)GUI_WINDOW::Hierarchy)->second);
	if (hw->_selected > -1 && hw->_selected < Scene::currentScene->sceneObjs.size()) {
		auto it = Scene::currentScene->sceneObjs.begin();
		std::advance(it, hw->_selected);
		go = &(*it);

		int n = 1;
		for (std::map<std::string, GameComponent*>::iterator it = go->components.begin(); it != go->components.end();) {
			std::string    key = it->first;
			GameComponent* gc = it->second;
			it++;
			if (!ImGui::CollapsingHeader(key.c_str())) continue;
			gc->Gui();
			if (!NWGui::Button("Delete")) continue;
			go->DeleteComponent(key);
			n++;
		}

		ImGui::NewLine();
		ImGui::NewLine();

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("popup0");
		}

		if (ImGui::BeginPopup("popup0")) {
			ADD_COMPONENT_TO_INSPECTOR(Transform, go);
			ADD_COMPONENT_TO_INSPECTOR(Sprite, go);
			ADD_COMPONENT_TO_INSPECTOR(Collider, go);
			ADD_COMPONENT_TO_INSPECTOR(Camera, go);
			ADD_COMPONENT_TO_INSPECTOR(ParticleSystem, go);
			ADD_COMPONENT_TO_INSPECTOR(AudioEmitter, go);
			ADD_COMPONENT_TO_INSPECTOR(AudioListener, go);
			ADD_COMPONENT_TO_INSPECTOR(Script, go);
			ADD_COMPONENT_TO_INSPECTOR(TextHandler, go);
			ImGui::EndPopup();
		}
	}
	ImGui::End();
};

#undef ADD_COMPONENT_TO_INSPECTOR