#pragma once
#include "NWGui.h"
#include "SceneEditorGui.h"
#include "Scene.h"
#include "Utilities.h"
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

#include "Text.h"
#include "Batch.h"
#include "TextHandler.h"
#include <iostream>

class DebugGui {
public:
	static bool isActive;
	static void Init() {}
	static void Show() {
		if (!isActive) return;
		ImGui::Begin("Debug", &isActive);

		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();
		static float point =  0.0f;
		static float point0 = 1.0f;
		static fVec2 point1 = fVec2(0.0, 0.0);
		static fVec2 point2 = fVec2(1.0, 0.0);
		NWGui::CubicBezierInterpolationPlot("Test", &point, &point0, &point1, &point2);
		ImGui::End();
	}
};
