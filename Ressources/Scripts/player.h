#pragma once
#include "Script.h"

class player : public Scriptable {
public:
	SCRIPT_CONSTR(player);

	void Start() {};
	void Update();
	void Gui() override;

private:
	float speed = 400.0f;
};
