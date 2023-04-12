#include "NWGui.h"
#include "Utilities.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot/implot.h"

uint32 NWGui::AutoInc     = 1;

static double xx[1000];
static double yy[1000];

void NWGui::Text(std::string text) {
	 ImGui::Text(text.c_str());
}

bool NWGui::FileHolder(std::string name, std::string holdedFile) {
	bool ret = 0;
	if (ImGui::BeginTable("testTable0", 2)) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(name.c_str());
		ImGui::TableNextColumn();
		ret = ImGui::Button(holdedFile.c_str(), ImVec2(150, 0));
		ImGui::EndTable();
	}
	return ret;
}

bool NWGui::CheckBox(std::string label, bool* value) {
	bool ret = 0;
	if (ImGui::BeginTable("testTable1", 2)) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(label.c_str());
		ImGui::TableNextColumn();
		ret = ImGui::Checkbox((std::string("##") + label).c_str(), value);
		ImGui::EndTable();
	}
	return ret;
}

bool NWGui::Button(std::string label) {
	return ImGui::Button(NWGui::GenLabel(label.c_str(), GET_GUI_ID).c_str());
}


bool NWGui::DragValue(const char* label, void* target, int dataType, int16 vectorSize, float speed, float minn, float maxx) {
	bool ret = 0;
	if (ImGui::BeginTable("testTable0", 2)) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(label);
		ImGui::TableNextColumn();
		ret = ImGui::DragScalarN(NWGui::GenLabel("", GET_GUI_ID).c_str(), dataType, target, vectorSize, speed, &minn, &maxx);
		ImGui::EndTable();
	}

	return ret;
}

std::string NWGui::GenLabel(const char* string, uint32 id) {
	return std::string(string) + std::string("##") + std::to_string(id);
}

bool NWGui::QuadraticBezierPlot(std::string label, fVec2* source, fVec2* target, fVec2* controlPoint) {
	dVec2 point = dVec2(source->x, source->y);
	dVec2 point0 = dVec2(target->x, target->y);
	dVec2 point1 = dVec2(controlPoint->x, controlPoint->y);

	for (int i = 1; i < 1001; i++) {
		float t = (float)(i) / 1001.0f;
		fVec2 temp = QbezierVector2<float, float>(*source, *target, *controlPoint, fVec2(t, t));
		yy[i - 1] = temp.y;
		xx[i - 1] = temp.x;
	}
	
#define FUNC(p, origin, r, g, b, a)  if (ImPlot::DragPoint(GET_GUI_ID, &p.x, &p.y, ImVec4(r, g, b, a)))  { origin->x = p.x; origin->y = p.y; }
	bool ret = 0;

	NWGui::DragValue((label + " (Source)").c_str(), &source->y, ImGuiDataType_Float, 1);
	NWGui::DragValue((label + " (Target)").c_str(), &target->y, ImGuiDataType_Float, 1);


	if (ImGui::TreeNode(GenLabel("Bezier Plot", GET_GUI_ID).c_str())) {
		if (ret = ImPlot::BeginPlot(NWGui::GenLabel(label.c_str(), GET_GUI_ID).c_str())) {
			FUNC(point, source, 1.0f, 0.0f, 0.0f, 1.0f);
			FUNC(point0, target, 0.0f, 0.0f, 1.0f, 1.0f);
			FUNC(point1, controlPoint, 0.0f, 1.0f, 0.0f, 1.0f);
			ImPlot::PlotLine(NWGui::GenLabel(label.c_str(), GET_GUI_ID).c_str(), xx, yy, 1000, 0);
			ImPlot::EndPlot();
		}
		NWGui::DragValue("ControlPoint", controlPoint, ImGuiDataType_Float, 2);
		ImGui::TreePop();
	}
	return ret;
#undef FUNC(p, origin, r, g, b, a)
}

bool NWGui::CubicBezierPlot(std::string label, fVec2* source, fVec2* target, fVec2* controlPoint, fVec2* controlPoint0) {
	dVec2 point    = dVec2(source->x, source->y);
	dVec2 point0   = dVec2(target->x, target->y);
	dVec2 point1   = dVec2(controlPoint->x,  controlPoint->y);
	dVec2 point2   = dVec2(controlPoint0->x, controlPoint0->y);

	for (int i = 1; i < 1001; i++) {
		float t = (float)(i) / 1001.0f;
		fVec2 temp = CbezierVector2<float, float>(*source, *target, *controlPoint, *controlPoint0, fVec2(t, t));
		yy[i - 1] = temp.y;
		xx[i - 1] = temp.x;
	}

#define FUNC(p, origin, r, g, b, a)  if (ImPlot::DragPoint(GET_GUI_ID, &p.x, &p.y, ImVec4(r, g, b, a)))  { origin->x = p.x; origin->y = p.y; }
	bool ret = 0;

	NWGui::DragValue( (label + " (Source)").c_str(), &source->y, ImGuiDataType_Float, 1);
	NWGui::DragValue( ("Target"), &target->y, ImGuiDataType_Float, 1);

	if (ImGui::TreeNode(GenLabel("Bezier Plot", GET_GUI_ID).c_str())) {
		if (ret = ImPlot::BeginPlot(NWGui::GenLabel(label.c_str(), GET_GUI_ID).c_str())) {
			FUNC(point1, controlPoint, 0.0f, 1.0f, 0.0f, 1.0f);
			FUNC(point2, controlPoint0, 0.0f, 1.0f, 0.0f, 1.0f);
			FUNC(point, source, 1.0f, 0.0f, 0.0f, 1.0f);
			FUNC(point0, target, 0.0f, 0.0f, 1.0f, 1.0f);
			ImPlot::PlotLine(NWGui::GenLabel(label.c_str(), GET_GUI_ID).c_str(), xx, yy, 1000, 0);
			ImPlot::EndPlot();
		}
		NWGui::DragValue("ControlPoint", controlPoint, ImGuiDataType_Float, 2);
		NWGui::DragValue("ControlPoint0",controlPoint0, ImGuiDataType_Float, 2);

		ImGui::TreePop();
	}
	return ret;
#undef FUNC(p, origin, r, g, b, a)
}

bool NWGui::QuadraticBezierInterpolationPlot(std::string label, float* source, float* target, fVec2* controlPoint) {
	fVec2 s  = fVec2(0.0f, *source);
	fVec2 t  = fVec2(1.0f, *target);
	bool ret = NWGui::QuadraticBezierPlot(label, &s, &t, controlPoint);
	*source  = s.y;
	*target  = t.y;
	return ret;
}

bool NWGui::CubicBezierInterpolationPlot(std::string label, float* source, float* target, fVec2* controlPoint, fVec2* controlPoint0) {
	fVec2 s = fVec2(0.0f , *source);
	fVec2 t = fVec2(1.0f , *target);
	bool ret = NWGui::CubicBezierPlot(label, &s, &t, controlPoint, controlPoint0);
	*source = s.y;
	*target = t.y;
	return ret;
}