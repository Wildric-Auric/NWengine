#pragma once
//IMPL_GUI wraps code for each component gui function overriden so that code is ignored when passing to game build
#ifdef NW_GUI_APP
	#define NW_IMPL_GUI(component, component_gui_code)  void component::Gui() {  component_gui_code };
	#define OVERRIDE_GUI void Gui() override;
#else
	#define NW_IMPL_GUI(component, component_gui_code)  //void component::Gui() { };
	#define OVERRIDE_GUI
#endif
	
class GuiObject {
public:
	virtual void Gui() {};
};