#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"
#include "ConsoleGui.h"



bool SceneViewGui::isActive			= 1;
bool DebugGui::isActive				= 1;
bool HierarchyGui::isActive			= 1;
bool InspectorGui::isActive			= 1;
bool SceneEditorGui::isActive		= 1;
bool ConsoleGui::isActive			= 1;


int16 HierarchyGui::selected = -1;
Camera SceneEditorGui::cam;
bool SceneEditorGui::f = 0;   // I hate this variable more than all my exes;  TODO::find a better solution (reminds me GMS scripting btw)