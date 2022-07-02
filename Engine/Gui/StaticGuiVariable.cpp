#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"




bool SceneViewGui::isActive			= 1;
bool DebugGui::isActive				= 1;
bool HierarchyGui::isActive			= 0;
bool InspectorGui::isActive			= 0;
bool SceneEditorGui::isActive		= 0;



int16 HierarchyGui::selected = -1;
Camera SceneEditorGui::cam;
bool SceneEditorGui::f = 0; 