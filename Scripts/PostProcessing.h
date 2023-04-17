#pragma once
#include "Script.h"
#include "Renderer.h"

class PostProcessing : public Scriptable {
public:
	SCRIPT_CONSTR(PostProcessing);
	~PostProcessing();
	void Start();
	void Update();
	void Gui() override;

private:
	GameObject container;
	Renderer*  renderer;
};
