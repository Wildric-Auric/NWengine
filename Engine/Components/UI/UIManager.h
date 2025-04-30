#pragma once
#include "Camera.h"
#include "GameObject.h"

// TODO::Add to gamecomponents ids and addcomp
class UIManager : public GameComponent {
  public:
	GameObject _camContainer;
	Camera*	   _lastCam = 0;

	Camera* GetCamera();
	Camera* GetTmpCamera();
	void	OnAdd() override;
	void	Update() override;
	void	Bind();
	void	Unbind();
};
