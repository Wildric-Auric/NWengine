#pragma once
#include "NWGui.h"


//URGENT TODO::NOT DEFINE THIS HERE!!
#define NW_GUI_APP
//IMPL_GUI wraps code for each component gui function overriden so that code is ignored when passing to game build
#ifdef NW_GUI_APP
	#define NW_IMPL_GUI(component, component_gui_code)  void component::Gui() {  component_gui_code };
#else
	#define NW_IMPL_GUI(component, component_gui_code)  void component::Gui() { };
#endif
	
class GuiObject {
public:
	virtual void Gui() {};
};