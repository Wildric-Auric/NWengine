#pragma once
#include "Components.h"

class SceneSet : public Script{
  public:
    GameObject* goc = 0;
	std::string		     __nwname = "SceneSet";                                                                                          \
	SceneSet(GameObject* goc) { this->goc = goc; };
	void Start() override;
	void Update() override;
};
