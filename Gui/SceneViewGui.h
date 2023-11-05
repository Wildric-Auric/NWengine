#pragma once

#include "GuiWindow.h"

#define SCENE_VIEW_PLAY_ICON "Ressources\\Images\\Icons\\Play.png"
#define SCENE_VIEW_NEXT_ICON "Ressources\\Images\\Icons\\Next.png"
#define SCENE_VIEW_PAUSE_ICON "Ressources\\Images\\Icons\\Pause.png"
#define SCENE_VIEW_STOP_ICON "Ressources\\Images\\Icons\\Stop.png"



class SceneViewGui : public GuiWindow {
public:
	void Init() override;
	void Show() override;
};
