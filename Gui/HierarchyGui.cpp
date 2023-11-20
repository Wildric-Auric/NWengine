#include "imgui/imgui.h"
#include "HierarchyGui.h"
#include "Scene.h"
#include "Inputs.h"

void HierarchyGui::Init() { this->isActive = 1; }
void HierarchyGui::Show() {
	static int32 renaming = -1;
	static char str0[128] = "new GameObject";
	if (!isActive) {
		_selected = -1;
		this->_selectedGameObject = nullptr;
		renaming = -1;
		return;
	}

	ImGui::Begin("Hierarchy", &isActive, ImGuiWindowFlags_MenuBar);
	if (Scene::currentScene == nullptr) {
		this->_selected = -1;
		this->_selectedGameObject = nullptr;
		ImGui::End();
		return;
	}

	ImGui::OpenPopupOnItemClick("createordelete", ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopupContextWindow("createordelete")) {

		if (ImGui::Selectable("new GameObject")) {
			Scene::currentScene->AddObject();
		}
		if (_selected != -1 && ImGui::Selectable("Delete GameObject")) {
			Scene::currentScene->DeleteObject(_selected);
			_selectedGameObject = nullptr;
			_selected = -1;
		}
		ImGui::EndPopup();
	}

	int8 i = 0;
	for (auto it = Scene::currentScene->sceneObjs.begin(); it != Scene::currentScene->sceneObjs.end(); it++) {
		if (renaming == i) {

			ImGui::InputText("input name", str0, IM_ARRAYSIZE(str0));
			if (Inputs::enter) {
				Scene::currentScene->Rename(std::string(str0), &*it);
				renaming = -1;
			}
		}
		else if (ImGui::Selectable(it->name.c_str(), _selected == i)) {
			_selected = i;
			_selectedGameObject = &*it;
			renaming = -1;
		}
		i++;
	}
	//Renaming
	if (_selected != -1) {
		if (Inputs::f2) {
			renaming = _selected;
		}
	}
	if (!ImGui::IsWindowFocused()) renaming = -1;
	ImGui::End();
}


GameObject* HierarchyGui::GetSelectedObject(int* index) {
	if (Scene::currentScene == nullptr) {
		this->_selectedGameObject = nullptr;
		this->_selected = 0;
		if (index != nullptr)
			*index = this->_selected;
		return nullptr;
	}
	if (index != nullptr)
		*index = this->_selected;
	return this->_selectedGameObject;
}