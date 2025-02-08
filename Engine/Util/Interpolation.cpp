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

void Interpolator::SetDuration(const float val) {
    duration = val;
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
 
void BezierInterpolator::SetCtrlPts(const fVec2& f, const fVec2& s) {
    controlPoints[0] = f;
    controlPoints[1] = s;
}

void BezierInterpolator::SetWithPreset(const fVec2& f, const fVec2& s) {
    source = 0.0f;
    target = 1.0f;
    SetCtrlPts(f,s);
}

void BezierInterpolator::SetFirstCtrlPt(const fVec2& p) {
    controlPoints[0] = p;
}

void BezierInterpolator::SetSecondCtrlPt(const fVec2& p) {
    controlPoints[1] = p;
}

BezierInterpolator2::BezierInterpolator2() {}

BezierInterpolator2::BezierInterpolator2(const fVec2& source, const fVec2& target, float duration) {
   this->source = source;
   this->target = target;
   this->duration = duration;
}

void  BezierInterpolator2::Linearize() {
    SetCtrlPts(source, source);
}

void  BezierInterpolator2::SetCtrlPts(const fVec2& f, const fVec2& s) {
    controlPoints[0] = f;
    controlPoints[1] = s;
}

void  BezierInterpolator2::SetFirstCtrlPt(const fVec2& p) {
    controlPoints[0] = p;
}

void  BezierInterpolator2::SetSecondCtrlPt(const fVec2& p) {
    controlPoints[1] = p;
}

void BezierInterpolator2::SetDuration(const float val) {
    duration = val; 
}

void BezierInterpolator2::Evaluate(float clock, fVec2* outVal) {
    currentValue = CbezierVector2(source, target, controlPoints[0], controlPoints[1], fVec2(clock, clock)).y;
    *outVal= currentValue;
}
