#ifndef MATHS_H
#define MATHS_H

class Vector2 {
public:
	float x;
	float y;
	Vector2(float x = 0.0f, float y = 0.0f);
private:
	Vector2 operator + (Vector2 const& vec1);
	Vector2 operator + (float const& num);
	Vector2 operator * (float const& num);
	Vector2 operator * (Vector2 const& vec1);

};
#endif