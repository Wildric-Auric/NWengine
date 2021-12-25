#ifndef MATHS_H
#define MATHS_H

template<class T>
class Vector2 {
public:
	float x;
	float y;
	Vector2(T x = 0.0f, T y = 0.0f);
private:
	Vector2 operator + (Vector2 const& vec1);
	Vector2 operator + (T const& num);
	Vector2 operator * (T const& num);
	Vector2 operator * (Vector2 const& vec1);
};



	template<class T>
	Vector2<T>::Vector2(T x, T y) {
		Vector2::x = x;
		Vector2::y = y;
	}
	template<class T>
	Vector2<T> Vector2<T>::operator + (Vector2 const& vec1) {
		Vector2 sum;
		sum.x = x + vec1.x;
		sum.y = y + vec1.y;
		return sum;
	}
	template<class T>
	Vector2<T> Vector2<T>::operator * (Vector2 const& vec1) {
		Vector2 newVec;
		newVec.x = x * vec1.x;
		newVec.y = y * vec1.y;
		return newVec;
	}
	template<class T>
	Vector2<T> Vector2<T>::operator + (T const& num) {
		Vector2 newVec;
		newVec.x = x + num;
		newVec.y = y + num;
		return newVec;
	}

	template<class T>
	Vector2<T> Vector2<T>::operator * (T const& num) {
		Vector2 newVec;
		newVec.x = x * num;
		newVec.y = y * num;
		return newVec;
	}


#endif