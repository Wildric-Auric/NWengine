#pragma once

#include "Script.h"

class Player : public Scriptable {

public:
	SCRIPT_CONSTR(Player);

	void Update() override;
	void Gui()    override;

	int  direction = 0;
private:
	float speed = 600.0f;

};