#include "Maths.h"

Vector2::Vector2(float x, float y) {
    Vector2::x = x;
    Vector2::y = y;
}

Vector2 Vector2::operator + (Vector2 const& vec1) {
    Vector2 sum;
    sum.x = x + vec1.x;
    sum.y = y + vec1.y;
    return sum;
}

Vector2 Vector2::operator * (Vector2 const& vec1) {
    Vector2 newVec;
    newVec.x = x * vec1.x;
    newVec.y = y * vec1.y;
    return newVec;
}

Vector2 Vector2::operator + (float const& num) {
    Vector2 newVec;
    newVec.x = x + num;
    newVec.y = y + num;
    return newVec;
}

Vector2 Vector2::operator * (float const& num) {
    Vector2 newVec;
    newVec.x = x * num;
    newVec.y = y * num;
    return newVec;
}