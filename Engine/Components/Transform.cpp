#include "Transform.h"

Transform::Transform(GameObject* go) {
	this->attachedObj = go;
}

void Transform::Gui() {
	static iVec2 guiPosition;
	guiPosition = position;
	if (NWGui::DragValue<int>("Position", &guiPosition.x, ImGuiDataType_S32, 2)) {
		position = guiPosition;
	};
	ImGui::Separator();
	NWGui::DragValue<float>("Scale", &scale.x, ImGuiDataType_Float, 2, 0.01f);
	ImGui::Separator();
}

std::map<GameObject*, Transform> Transform::componentList;