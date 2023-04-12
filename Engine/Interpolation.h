#pragma once
#include "Globals.h"
#include "Serialized.h"
#include "GuiObject.h"
#include <vector>



class Interpolator : public Serialized, public GuiObject {
public:
	
	struct {
		float source;
		float target;
		float currentValue;
	};

	float duration = 0.0f;
	
	Interpolator(float source, float target, float duration);
	Interpolator() {};
	//Clock between 0 and 1
	virtual float Evaluate(float clock);

	void Gui()										 override;
	int  Serialize(std::fstream* data, int offset)   override;
	int  Deserialize(std::fstream* data, int offset) override;
};


class BezierInterpolator : public Interpolator {
public:
	fVec2 controlPoints[2];

	BezierInterpolator(float source, float target, float duration);
	BezierInterpolator() {}

	void Linearize();

	float Evaluate(float clock) override;
	void Gui() override;
	int  Serialize(std::fstream* data, int offset) override;
	int  Deserialize(std::fstream* data, int offset) override;
};