#pragma once
#include "Script.h"


class Ball : public Scriptable {

public:
	SCRIPT_CONSTR(Ball);

	void Update() override;
	void Gui()    override;

	fVec2 direction = fVec2(1.0, 0.0);
	float speed = 500.0f;
};