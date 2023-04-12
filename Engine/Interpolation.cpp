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


void Interpolator::Gui() {
	NWGui::DragValue("Source", &this->source, GuiDataType_Float, 1);
	NWGui::DragValue("Target", &this->target, GuiDataType_Float, 1);
	NWGui::DragValue("Duration", &this->duration, GuiDataType_Float, 1);
};

int  Interpolator::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;

	WRITE_ON_BIN(data, &this->source, sizeof(this->source), sizeBuffer);
	WRITE_ON_BIN(data, &this->target, sizeof(this->target), sizeBuffer);
	WRITE_ON_BIN(data, &this->duration, sizeof(this->duration), sizeBuffer);
	return 0;
}

int  Interpolator::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	READ_FROM_BIN(data, &this->source, sizeBuffer);
	READ_FROM_BIN(data, &this->target, sizeBuffer);
	READ_FROM_BIN(data, &this->duration, sizeBuffer);

	this->currentValue = this->source;
	return 0;
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

void BezierInterpolator::Gui() {
	NWGui::CubicBezierInterpolationPlot("", &this->source, &this->target, &this->controlPoints[0], &this->controlPoints[1]);	
	NWGui::DragValue("Duration", &this->duration, GuiDataType_Float, 1);
};

int  BezierInterpolator::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, &this->source, sizeof(this->source), sizeBuffer);
	WRITE_ON_BIN(data, &this->target, sizeof(this->target), sizeBuffer);

	WRITE_ON_BIN(data, &this->controlPoints[0].x, sizeof(float), sizeBuffer); //TODO::Add serialization for vector
	WRITE_ON_BIN(data, &this->controlPoints[0].y, sizeof(float), sizeBuffer);

	WRITE_ON_BIN(data, &this->controlPoints[1].x, sizeof(float), sizeBuffer);
	WRITE_ON_BIN(data, &this->controlPoints[1].y, sizeof(float), sizeBuffer);

	WRITE_ON_BIN(data, &this->duration, sizeof(this->duration), sizeBuffer);
	return 0;
}

int  BezierInterpolator::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	READ_FROM_BIN(data, &this->source, sizeBuffer);
	READ_FROM_BIN(data, &this->target, sizeBuffer);

	READ_FROM_BIN(data, &this->controlPoints[0].x, sizeBuffer);
	READ_FROM_BIN(data, &this->controlPoints[0].y, sizeBuffer);

	READ_FROM_BIN(data, &this->controlPoints[1].x, sizeBuffer);
	READ_FROM_BIN(data, &this->controlPoints[1].y, sizeBuffer);

	READ_FROM_BIN(data, &this->duration, sizeBuffer);

	this->currentValue = this->source;
	return 0;
}


