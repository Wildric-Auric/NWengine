#pragma once
#include "NWGui.h"
#include "SceneEditorGui.h"
#include "Scene.h"
#include "Utilities.h"
#include "PostProcessing.h"
#include "Console.h"
#include "ScriptManager.h"
#include "ParticleSystem.h"
#include "imgui\\implot\\implot.h"

#include "Serializer.h"

#include "AudioListener.h"
#include "AudioEmitter.h"

#include "Builder.h"

#include "ScriptManagerGui.h"
#include "Collider.h"

class DebugGui {
public:
	static bool isActive;
	static void Show() {

		
		if (!isActive) return;



		ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);

		NWGui::DragValue("Stretch", &Renderer::currentRenderer->strechCoeff.x, ImGuiDataType_Float, 2, 0.1, 0.0f, 2.0f);

		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();
		static dVec2 point =  dVec2(0.0, 0.0);
		static dVec2 point0 = dVec2(1.0, 0.0);
		static double xx[1000];
		static double yy[1000];
		for (int i = 1; i < 1001; i++) {
			fVec2 temp = CbezierVector2(dVec2(0.0f, 0.0f), dVec2(1.0f,1.0f), point, point0, fVec2(((float)i)/1000.0f, ((float)i)/1000.0f));
			xx[i-1] = temp.x;
			yy[i-1] = temp.y;
		}
		if (ImPlot::BeginPlot("Hello World")) {
			ImPlot::DragPoint(1504, &point.x, &point.y, ImVec4(1.0f,0.0f,0.0f, 1.0f));
			ImPlot::DragPoint(2069, &point0.x, &point0.y, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImPlot::PlotLine("bezier", xx, yy, 1000, ImPlotLineFlags_Segments);
			ImPlot::EndPlot();
		}
		ImGui::End();
	}
};
