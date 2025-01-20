#include "Interpolation.h"


Interpolator::Interpolator(float source, float target, float duration) {
	this->source	   = source;
	this->target	   = target;
	this->currentValue = source;

	this->duration = duration;
}

float Interpolator::Evaluate(float clock) {
	return currentValue = lerp(this->source, this->target, clock);
}

BezierInterpolator::BezierInterpolator(float source, float target, float duration)  : Interpolator(source, target, duration){
	Linearize();
}

void BezierInterpolator::Linearize() {
	this->controlPoints[0].y = this->source;
	this->controlPoints[0].x = 0.0f;

	this->controlPoints[1].y = this->source;
	this->controlPoints[1].x = 0.0f;

}

float BezierInterpolator::Evaluate(float clock) {
	fVec2 s = fVec2(0.0f, this->source);
	fVec2 e = fVec2(1.0f, this->target);
	return this->currentValue = CbezierVector2(s, e, controlPoints[0], controlPoints[1], fVec2(clock, clock)).y; //TODO::This in one dimension only
}