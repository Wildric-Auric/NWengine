#pragma once
#include "GameObject.h"
#include "Globals.h"

class CircleRenderer : public GameComponent {
  public:
	float _aaValue = 0.0;
	NW_ST_GET_TYPE_IMPL(CircleRenderer);

	CircleRenderer() {};
	CircleRenderer(GameObject*);

	void  SetRadius(float);
	void  SetRenderingAA(float);
	void  SetPosition(const fVec2&);
	float GetRadius();
};
