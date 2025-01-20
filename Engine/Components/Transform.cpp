#include "Transform.h"
#include "Maths.h"

Transform::Transform(GameObject* go) {
	this->attachedObject = go;
}

fVec2& Transform::Translate(const fVec2& tr) {
    position = position + tr;
    return position;
}

fVec2& Transform::Scale(const fVec2& s) {
    scale = scale + s;
    return scale;
}

fVec2& Transform::Lerp(const fVec2& start, const fVec2& end, const fVec2& ratio) {
    position = lerpVector2(start, end, ratio);
    return position;
}

float Transform::Rotate(const float r) { 
    rotation += r;
    return r;
}

void Transform::SetPosition(const fVec2& pos) {position = pos;}
void Transform::SetScale(const fVec2& s) {scale = s;}
void Transform::SetRotation(const float r) {rotation = r;}
fVec2 Transform::GetPosition() {return position;}
fVec2 Transform::GetScale() {return scale;}
float Transform::GetRotation() {return rotation;}