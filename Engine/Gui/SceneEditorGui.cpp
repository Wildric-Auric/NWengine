#include "imgui/imgui.h"
#include "imgui/imguizmo/ImGuizmo.h"
#include "glm/gtx/matrix_decompose.hpp"


#include "Scene.h"
#include "Transform.h"
#include "Inputs.h"
#include "SceneEditorGui.h"
#include "SceneEditor.h"
#include "Gui.h"
#include "HierarchyGui.h"

//TODO::Make a class for guizmos

void SceneEditorGui::Init() { this->isActive = 1; }
void SceneEditorGui::Show() {
	if (!isActive) return;
	static bool onpress = 0;
	static iVec2 pos0 = 0;
	static iVec2 camPos;
	static bool snap = 0;
	static float snapValue = 1.0f;
	snap = Inputs::ctrl;
	//SetUp-------------------------------
	ImGui::Begin("Scene Editor", &isActive, ImGuiWindowFlags_MenuBar);
	ImGuizmo::SetOrthographic(1);
	ImGuizmo::SetDrawlist();

	if (SceneEditor::cam == nullptr) return;
	Camera& cam = *SceneEditor::cam;
	SceneEditor::Update();
	ImGuiIO& io = ImGui::GetIO();

	iVec2 mousePosition = iVec2(io.MousePos.x - ImGui::GetCursorScreenPos().x, cam.size.y - io.MousePos.y + ImGui::GetCursorScreenPos().y);
	bool focused = ImGui::IsWindowFocused();
	ImVec2 offset = ImGui::GetCursorPos();
	ImGui::Image((void*)(intptr_t)
		cam.fbo.RenderedImage.texture,
		ImVec2(cam.fbo.RenderedImage.size.x, cam.fbo.RenderedImage.size.y),
		ImVec2(0, 1), ImVec2(1, 0));

	//Guizmo Logic------------------------
	static ImGuizmo::OPERATION guizmoOp = ImGuizmo::OPERATION::TRANSLATE_X | ImGuizmo::OPERATION::TRANSLATE_Y;
	if (focused) {
		if (Inputs::r) 
			guizmoOp = ImGuizmo::OPERATION::ROTATE_Z;
		else if (Inputs::s)
			guizmoOp = ImGuizmo::OPERATION::SCALE_X | ImGuizmo::OPERATION::SCALE_Y;
		else if (Inputs::t)
			guizmoOp = ImGuizmo::OPERATION::TRANSLATE_X | ImGuizmo::OPERATION::TRANSLATE_Y;
	}

	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 scroll = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());
	ImGuizmo::SetRect(winPos.x + offset.x - scroll.x, winPos.y + offset.y - scroll.y, cam.fbo.RenderedImage.size.x, cam.fbo.RenderedImage.size.y);

	GameObject* obj = ((HierarchyGui*)Gui::Windows[(uint32)GUI_WINDOW::Hierarchy])->GetSelectedObject();
	bool guizmoSelected = 0;
	Transform* transform;
	if (obj != nullptr && (transform = obj->GetComponent<Transform>()) != nullptr) {
		snapValue		= 8.0f;
		fVec2 pos		= transform->position;
		glm::vec4 v		= glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
		glm::mat4 mat   = glm::mat4(1.0);
		mat[3][0] = v.x; mat[3][1] = v.y; mat[3][2] = -1.0f; mat[3][3] = 1.0f;
		static fVec2 sizeBuffer;

		ImGuizmo::Manipulate((const float*)&cam.viewMatrix[0][0],(const float*)&cam.projectionMatrix[0][0], guizmoOp, ImGuizmo::LOCAL, (float*)(&mat[0]),
			0, snap ? &snapValue : 0);
		if (ImGuizmo::IsUsing()) {
			fVec3 scale, trans, rot;
			ImGuizmo::DecomposeMatrixToComponents(&mat[0][0], &trans.x, &rot.x, &scale.x);
			transform->position    =  fVec2(trans.x, trans.y);
			transform->scale       =  sizeBuffer * fVec2(scale.x, scale.y);
			transform->rotation   +=  rot.z;	
		}
		else {
			sizeBuffer = obj->GetComponent<Transform>()->scale;
		}
	}
	

	//Scrolling logic-----------------------------------
	if (!onpress && focused && Inputs::left_click) {
		onpress = 1;
		pos0 = mousePosition;
		camPos = cam.position;
	}
	if (!focused || !Inputs::left_click || ImGuizmo::IsUsing()) onpress = 0;
	if (onpress) cam.position = camPos - mousePosition + pos0;
	//End------------------------
	ImGui::End();
};
