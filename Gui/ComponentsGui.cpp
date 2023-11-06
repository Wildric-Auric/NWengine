#include "Components.h"
#include "Camera.h"
#include "Utilities.h"
#include "ScriptManager.h"
#include "Interpolation.h"
#include "NWGui.h"
#include "imgui/imgui.h"

void Interpolator::Gui() {
	NWGui::DragValue("Source", &this->source, GuiDataType_Float, 1);
	NWGui::DragValue("Target", &this->target, GuiDataType_Float, 1);
	NWGui::DragValue("Duration", &this->duration, GuiDataType_Float, 1);
};

void BezierInterpolator::Gui() {
	NWGui::CubicBezierInterpolationPlot("", &this->source, &this->target, &this->controlPoints[0], &this->controlPoints[1]);
	NWGui::DragValue("Duration", &this->duration, GuiDataType_Float, 1);
};

void Sprite::Gui() {
	if (NWGui::DragValue("Layering Order", &sortingLayer, ImGuiDataType_U32, 1, 1.0f, 0.0f, 6000.0f))
		SetSortingLayer(sortingLayer);
	ImGui::Separator();
	if (NWGui::FileHolder("Texture", texture->name)) {
		std::string path = GetFile(WIN_STR_FILTER("Image files", "*.png;*.jpeg;*.jpg"));
		if (path != "") SetTexture(path);
	}
	ImGui::Separator();
	if (NWGui::FileHolder("Shader", shader->name)) {
		std::string path = GetFile(WIN_STR_FILTER("Shader files", "*.shader"));
		if (path != "") SetShader(path);
	}
	if (ImGui::Button("Recompile Shader"))
		RessourcesLoader::ReloadShader(this->shader->name);

	ImGui::Checkbox("isBatched", &isBatched);

	ImGui::Separator();
}

void Transform::Gui() {
	static iVec2 guiPosition;
	guiPosition = position;
	if (NWGui::DragValue("Position", &guiPosition.x, ImGuiDataType_S32, 2)) {
		position = guiPosition;
	};
	ImGui::Separator();
	NWGui::DragValue("Scale", &scale.x, ImGuiDataType_Float, 2, 0.01f);
	NWGui::DragValue("Rotation", &rotation, ImGuiDataType_Float, 1, 1.0);
	ImGui::Separator();
}

void TextHandler::Gui() {
	if (NWGui::FileHolder("Font", this->font.name)) {
		std::string path = GetFile(WIN_STR_FILTER("Font files", "*.ttf"));
		this->SetFont(path);
	}

	if (NWGui::FileHolder("Shader", this->shader)) {
		std::string path = GetFile(WIN_STR_FILTER("Shader files", "*.shader"));
		if (path != "") { this->shader = path; }
	}
	NWGui::CheckBox("isBatched", &this->isBatched);
	NWGui::DragValue("Position", &this->position, GuiDataType_Float, 2, 1.0);
	NWGui::DragValue("Scale", &this->scale, GuiDataType_Float, 2, 0.05f, 0.0f, 100.0f);
	NWGui::DragValue("Color", &this->colors, GuiDataType_Float, 4, 0.05f, 0.0f, 1.0f);
	NWGui::Input("Text", &this->text);
	if (NWGui::Button("Update characters"))
		this->UpdateGlyphs();
}

void Script::Gui() {
	std::string text = "None";
	if (this->script != nullptr) text = this->script->GetName();
	if (NWGui::FileHolder("Script", text.c_str())) {
		std::string path = GetFile(WIN_STR_FILTER("Script files", "*.h"));
		std::string filename = "";
		std::string root = "";
		GetFileName(path, &filename, nullptr, &root);
		if (path == "") return;
		script = ScriptManager::CreateScript(filename, attachedObj); //TODO::Get if file is valid
		ScriptManager::scriptList.insert(std::make_pair(filename, root));
	}

	if (this->script != nullptr)
		this->script->Gui();
}

void ParticleSystem::Gui() {
	ImGui::Checkbox("Active", &isActive);
	if (ImGui::TreeNode("Particles properties")) {
		GUI_SEP
			NWGui::DragValue("Lifetime", &prop.lifetime, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("Lifedistance", &prop.lifedistance, ImGuiDataType_S32);
		GUI_SEP;
		NWGui::DragValue("Relative start position", &prop.sPosition, ImGuiDataType_Float, 2);
		GUI_SEP;

		GUI_NODE_BEG("Horizontal direction")
			prop.directionX.Gui();
		GUI_NODE_END

			GUI_NODE_BEG("Vertical direction")
			prop.directionY.Gui();
		GUI_NODE_END
			GUI_SEP;

		GUI_NODE_BEG("Horizontal scale")
			prop.scaleX.Gui();
		GUI_NODE_END;

		GUI_NODE_BEG("Vertical scale")
			prop.scaleY.Gui();
		GUI_NODE_END
			GUI_SEP;
		NWGui::Text("Color: Red channel");
		prop.colorX.Gui();
		NWGui::Text("Color: Green channel");
		prop.colorY.Gui();
		NWGui::Text("Color: Blue channel");
		prop.colorZ.Gui();
		NWGui::Text("Color: Alpha channel");
		prop.colorA.Gui();
		GUI_SEP;
		NWGui::Text("Speed");
		prop.speed.Gui();
		GUI_SEP;
		GUI_SEP; GUI_NEWLINE;
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Emission properties")) {

		if (NWGui::FileHolder("Shader", shader)) {
			std::string path = GetFile(WIN_STR_FILTER("Shader files", "*.shader"));
			if (path != "") this->shader = path;
		}
		GUI_SEP

			if (NWGui::FileHolder("Texture", this->texture)) {
				std::string path = GetFile(WIN_STR_FILTER("Image files", "*.png;*.jpeg;*.jpg"));
				if (path != "") this->texture = path;
			}
		GUI_SEP
			NWGui::DragValue("Emission frequency", &emissionFrequency, ImGuiDataType_Double);
		GUI_SEP;
		NWGui::DragValue("Emission quantity", &emissionQuantity, ImGuiDataType_U16);
		GUI_SEP;
		NWGui::CheckBox("Recycle particles", &recycle);
		GUI_SEP;
		NWGui::DragValue("Max particles(experimental)", &maxParticles, ImGuiDataType_S32);
		ImGui::TreePop();
	}
}

void Collider::Gui() {
	if (ImGui::Button("Update Size")) {
		Start();
	}
}

void AudioEmitter::Gui() {
	std::string temp = "";
	if (sound != nullptr) temp = sound->name;

	if (NWGui::FileHolder("Sound", temp)) {
			std::string path = GetFile(WIN_STR_FILTER("Sound Files", "*.wav;*.ogg;*.flac"));
			if (path == "") return;

			if (sound == nullptr) { sound = new Sound(path); return; } //TODO::Add Sound function
			if (sound->name == path) return;
			delete sound;
			sound = new Sound(path);
			//TODO::Check only the file name
		}

		if (sound == nullptr) return;
		bool temp0 = sound->isPlaying;
		if (NWGui::CheckBox("isPlaying", &temp0)) {
			if (!sound->isPlaying)
				sound->Play();
			else
				sound->Stop();
			temp0 = !temp0;
		}

		NWGui::DragValue("Volume", &this->volume, ImGuiDataType_S32, 1, 1, 0.0f, 100.0f);
		NWGui::DragValue("Frequency", &this->frequency, ImGuiDataType_Float, 1, 0.1f, 0.0f, 2.0f);
		NWGui::CheckBox("Loop", &this->isLooping);
}

void Camera::Gui() {

	if (NWGui::CheckBox("Activate", &isActive)) {
		if (isActive) { Use(); }
		else {
			Camera::ActiveCamera = nullptr;
			this->isActive = 0;
		}
	};

	NWGui::DragValue("Camera Position", &position.x, ImGuiDataType_Float, 2, 1.0f);
	NWGui::DragValue("Camera Rotation", &rotation, ImGuiDataType_Float, 1);
	NWGui::DragValue("Camera Zoom", &zoom, ImGuiDataType_Float, 1, 0.1f, 0.0f, 100.0f);
	NWGui::DragValue("Clearing color", &clearColor.x, ImGuiDataType_Float, 3, 0.1, 0.0f, 1.0f);
	NWGui::DragValue("Resolution", &viewPortSize.x, ImGuiDataType_Float, 2, 10.0f, 0.0f, 10000.0f);
	if (NWGui::Button("Apply"))
		this->ChangeOrtho(viewPortSize.x, viewPortSize.y);
}
