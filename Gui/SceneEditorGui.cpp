#include "imgui/imgui.h"
#include "imgui/imguizmo/ImGuizmo.h"
#include "imgui/implot/implot.h"
#include "Scene.h"
#include "Transform.h"
#include "Inputs.h"
#include "SceneEditorGui.h"
#include "SceneEditor.h"
#include "Gui.h"
#include "HierarchyGui.h"


//TODO::Make a class for guizmos
#include "imgui/implot/implot.h"
#include "imgui/implot/implot_internal.h"

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
		//if (SceneEditor::cam == nullptr) return;
	Camera& cam = *SceneEditor::cam;
	ImGui::Begin("Scene Editor", &isActive);

	if (ImPlot::BeginPlot("Scene Plot", ImVec2(cam.fbo.RenderedImage.size.x, cam.fbo.RenderedImage.size.y), ImPlotFlags_NoTitle)) {

	ImVec2 offset = ImGui::GetCursorPos();
	ImGuizmo::SetOrthographic(1);
	ImGuizmo::SetDrawlist();

	SceneEditor::Update();
	ImGuiIO& io = ImGui::GetIO();
	iVec2 mousePosition = iVec2(io.MousePos.x - ImGui::GetCursorScreenPos().x, cam.size.y - io.MousePos.y + ImGui::GetCursorScreenPos().y);
	bool focused = ImGui::IsWindowFocused();

	ImPlot::PlotImage("fierfi", (ImTextureID)cam.fbo.RenderedImage.texture, 
					  ImPlotPoint(ImPlotPoint(-cam.fbo.RenderedImage.size.x / 2 + cam.position.x, -cam.fbo.RenderedImage.size.y / 2 + cam.position.y)), 
					  ImPlotPoint(cam.fbo.RenderedImage.size.x / 2 + cam.position.x, cam.fbo.RenderedImage.size.y /2 + cam.position.y), ImVec2(0, 1), ImVec2(1, 0));

	static dVec2 p(0.0,0.0);

	ImPlot::DragPoint(GET_GUI_ID, &p.x, &p.y, ImVec4(1.0, 1.0, 1.0, 1.0));

	static double xs[2] = {0., 100.};
	static double ys[2] = {.0, 100.0};
	ImPlot::SetNextMarkerStyle(ImPlotMarker_Right, 10.0);
	
	ImPlot::DragPoint(10002934, &xs[1], &ys[1], ImVec4(1.0,0.0,0.0,1.0), 10.0f);

	ImPlot::PlotLine("##Filled", xs, ys, 2);

	ImPlotRect plotScrollData = ImPlot::GetPlotLimits();
	
	cam.position.x			  = (plotScrollData.X.Min + plotScrollData.X.Max) / 2;
	cam.position.y			  = (plotScrollData.Y.Min + plotScrollData.Y.Max) / 2;
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
	ImGuizmo::SetRect(winPos.x + offset.x - scroll.x, winPos.y - scroll.y, cam.fbo.RenderedImage.size.x, cam.fbo.RenderedImage.size.y);

	GameObject* obj = ((HierarchyGui*)Gui::Windows[(uint32)GUI_WINDOW::Hierarchy])->GetSelectedObject();
	bool guizmoSelected = 0;
	Transform* transform;
	if (obj != nullptr && (transform = obj->GetComponent<Transform>()) != nullptr) {
		snapValue		= 8.0f;
		fVec2 pos		= transform->position;
		static fVec2 sizeBuffer;


		cam.Update();
		//ImGuizmo::Manipulate((const float*)&cam.viewMatrix[0][0],(const float*)&cam.projectionMatrix[0][0], guizmoOp, ImGuizmo::LOCAL, (float*)(&mat[0]),
		//	0, snap ? &snapValue : 0);
		
		//cam.Update();
		//if (ImGuizmo::IsUsing()) {
		//	fVec3 scale, trans, rot;
		//	//ImGuizmo::DecomposeMatrixToComponents(&mat[0][0], &trans.x, &rot.x, &scale.x);
		//	transform->position    =  fVec2(trans.x, trans.y);
		//	transform->scale       =  sizeBuffer * fVec2(scale.x, scale.y);
		//	transform->rotation   +=  rot.z;	
		//}
		//else {
		//	sizeBuffer = obj->GetComponent<Transform>()->scale;
		//}
	}
	

	////Scrolling logic-----------------------------------
	//if (!onpress && focused && Inputs::left_click) {
	//	onpress = 1;
	//	pos0 = mousePosition;
	//	camPos = cam.position;
	//}
	//if (!focused || !Inputs::left_click || ImGuizmo::IsUsing()) onpress = 0;
	//if (onpress) cam.position = camPos - mousePosition + pos0;
	//End------------------------
	ImPlot::EndPlot();
	
	}
	ImGui::End();
};
