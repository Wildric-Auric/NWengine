#pragma once
#include <iostream>
#include <cmath>



#define PI 3.14159265359
#define ONEDIV180 0.00555555555


inline float DegToRad(float degree) {
	return degree * PI * ONEDIV180;
}





template<typename T>
class Vector2 {
public:
	T x;
	T y;
	Vector2(T x = 0.0f, T y = 0.0f);

	Vector2<float> normalize();
	float magnitude();
	T Dot(Vector2 const& vec1);
	Vector2 Project(Vector2 const& vec1);
	Vector2 Rotate(float const& angle); //In degree

	Vector2 operator + (Vector2 const& vec1);
	Vector2 operator + (T const& num);
	Vector2 operator - (Vector2 const& vec1);
	Vector2 operator - (T const& num);
	template<typename T1>
	Vector2 operator * (T1 const& num);
	Vector2 operator * (Vector2 const& vec1);
	bool operator == (Vector2 const& vec1);
	bool operator != (Vector2 const& vec1);
	
	operator Vector2<int>() const { return Vector2<int>(x,y); }
	operator Vector2<float>() const { return Vector2<float>(x, y); }

	T* operator [] (int index);
};


template<typename T>
Vector2<float> Vector2<T>::normalize() {
	float magnitude = pow(x * x + y * y, 0.5);
	if (magnitude == 0) {
		return Vector2<float>(0, 0);
	}
	return Vector2<float>(x / magnitude, y / magnitude);
}
template<typename T>
float Vector2<T>::magnitude() {
	return pow(x * x + y * y, 0.5);
}

template<typename T>
T Vector2<T>::Dot(Vector2 const& vec1) {
	return x * vec1.x + y * vec1.y;
};

template<typename T>
//Vec1 should be normalized
Vector2<T> Vector2<T>::Project(Vector2 const& vec1) {
	return Dot(vec1) * vec1;
}

template<typename T> 
Vector2<T> Vector2<T>::Rotate(float const& angle) {
	float angle0 = DegToRad(angle);
	return Vector2<T>(cos(angle0) * x - sin(angle0) * y, 
					  sin(angle0) * x + cos(angle0) * y);
}
template<typename T>
Vector2<T>::Vector2(T x, T y) {
	Vector2::x = x;
	Vector2::y = y;
}
template<typename T>
Vector2<T> Vector2<T>::operator + (Vector2 const& vec1) {
	Vector2 sum;
	sum.x = x + vec1.x;
	sum.y = y + vec1.y;
	return sum;
}
template<typename T>
Vector2<T> Vector2<T>::operator - (Vector2 const& vec1) {
	Vector2 sub;
	sub.x = x - vec1.x;
	sub.y = y - vec1.y;
	return sub;
}

template<class T>
Vector2<T> Vector2<T>::operator * (Vector2 const& vec1) {
	Vector2 newVec;
	newVec.x = x * vec1.x;
	newVec.y = y * vec1.y;
	return newVec;
}
template<typename T> 
Vector2<T> Vector2<T>::operator + (T const& num) {
	Vector2 newVec;
	newVec.x = x + num;
	newVec.y = y + num;
	return newVec;
}
template<typename T>
Vector2<T> Vector2<T>::operator - (T const& num) {
	Vector2 newVec;
	newVec.x = x - num;
	newVec.y = y - num;
	return newVec;
}

template<typename T>
template<typename T1>
Vector2<T> Vector2<T>::operator * (T1 const& num) {
	Vector2 newVec;
	newVec.x = x * num;
	newVec.y = y * num;
	return newVec;
}
			
template<typename T>
bool Vector2<T>::operator == (Vector2 const& vec1) {
	return (x == vec1.x) && (y == vec1.y);
}

template<typename T>
bool Vector2<T>::operator != (Vector2 const& vec1) {
	return (x != vec1.x) && (y != vec1.y);
}

template<typename T> 
T* Vector2<T>::operator [] (int index) {
	return  index == 0 ? &x : &y;
}



template<typename T> 
int Sign(T number) {
	if (number < 0) return -1;
		return 1;
}

template<typename T> 
T Min(T x, T y) {
	return x <= y ? x : y;
};

template<typename T>
T Max(T x, T y) {
	return x >= y ? x : y;
};

template<typename T>
T Clamp(T num, T min, T max) {
	if (num < min) return min;
	else if (num > max) return max;
	return num;
}

template<typename T>
class Vector3 {
public:
	T x;
	T y;
	T z;
	Vector3(T x = 0.0f, T y = 0.0f, T z = 0.0f);

	Vector3<float> normalize();
	float magnitude();
	T Dot(Vector3 const& vec1);

	Vector3 Project(Vector3 const& vec1);

	Vector3 operator + (Vector3 const& vec1);
	Vector3 operator + (T const& num);
	Vector3 operator * (T const& num);
	Vector3 operator * (Vector3 const& vec1);
	bool operator == (Vector3 const& vec1);
	bool operator != (Vector3 const& vec1);
};

template<typename T>
Vector3<float> Vector3<T>::normalize() {
	float magnitude = pow(x * x + y * y + z * z, 0.5);
	return Vector3<float>(x / magnitude, y / magnitude, z / magnitude);
}
template<typename T>
float Vector3<T>::magnitude() {
	return pow(x * x + y * y + z * z,0.5);
}
template<typename T>
T Vector3<T>::Dot(Vector3 const& vec1) {
	return x * vec1.x + y * vec1.y + z * vec1.z;
};

template<typename T>
//Vec1 should be normalized
Vector3<T> Vector3<T>::Project(Vector3 const& vec1) {
	return vec1; //TODO::
}

template<class T>
Vector3<T>::Vector3(T x, T y, T z) {
	Vector3::x = x;
	Vector3::y = y;
	Vector3::z = z;
}
template<typename T>
Vector3<T> Vector3<T>::operator + (Vector3 const& vec1) {
	Vector3 sum;
	sum.x = x + vec1.x;
	sum.y = y + vec1.y;
	sum.z = z + vec1.z;
	return sum;
}
template<typename T>
Vector3<T> Vector3<T>::operator * (Vector3 const& vec1) {
	Vector3 newVec;
	newVec.x = x * vec1.x;
	newVec.y = y * vec1.y;
	newVec.z = z * vec1.z;
	return newVec;
}
template<typename T>
Vector3<T> Vector3<T>::operator + (T const& num) {
	Vector3 newVec;
	newVec.x = x + num;
	newVec.y = y + num;
	newVec.z = z + num;
	return newVec;
}

template<typename T>
Vector3<T> Vector3<T>::operator * (T const& num) {
	Vector3 newVec;
	newVec.x = x * num;
	newVec.y = y * num;
	newVec.z = z * num;
	return newVec;
}
template<typename T>
bool Vector3<T>::operator == (Vector3 const& vec1) {
	return (x == vec1.x) && (y == vec1.y) && (z == vec1.z);
}

template<typename T>
bool Vector3<T>::operator != (Vector3 const& vec1) {
	return (x != vec1.x) && (y != vec1.y) && (z != vec1.z);
}

template<uint8_t n, uint8_t m, typename T>
class Matrix {
public:
	Vector2<int> dimension;
	T coeff[n*m];
	Matrix(T* data);
	Matrix operator + (T const& num);
	Matrix operator * (T const& num);
	Matrix operator + (Matrix* const& matrix);
	void operator += (T const& num);
	void operator *= (T const& num);
	void operator += (Matrix* const& matrix);
	template<uint8_t b>
	Matrix<n, b, T> operator * (Matrix<m, b, T>* const& matrix);
	//void operator *= (Matrix<m, b, T>* const& matrix); Can you do it without creating another matrix??

};

template<uint8_t n, uint8_t m, typename T>
Matrix<n, m, T>::Matrix(T* data) {
	dimension.x = n;
	dimension.y = m;
	int size = sizeof(T) * n * m;
	if (data!=nullptr) memcpy(coeff, data, size);
	else {
		for (int i = 0; i < n * m; i++) {
			coeff[i] = (T)0;
		};
	}

};

template<uint8_t n, uint8_t m, typename T>
Matrix<n, m, T> Matrix<n, m, T>::operator + (T const& num) {
	Matrix<n, m, T> result = Matrix<n, m, T>(coeff);
	for (uint8_t i = 0; i < n * m; i++) {
		result.coeff[i] += num;
	};
	return result;
}

template<uint8_t n, uint8_t m, typename T>
void Matrix<n, m, T>::operator += (T const& num) {
	for (uint8_t i = 0; i < n * m; i++) {
		coeff[i] += num;
	};
}

template<uint8_t n, uint8_t m, typename T>
Matrix<n,m,T> Matrix<n, m, T>::operator * (T const& num) {
	Matrix<n,m,T> result = Matrix<n,m,T>(coeff);
	for (uint8_t i = 0; i < n * m; i++) {
		result.coeff[i] *= num;
	};
	return result;
}

template<uint8_t n, uint8_t m, typename T>
void Matrix<n, m, T>::operator *= (T const& num) {
	for (uint8_t i = 0; i < n * m; i++) {
		coeff[i] *= num;
	};
}

template<uint8_t n, uint8_t m, typename T>
Matrix<n, m, T> Matrix<n, m, T>::operator + (Matrix* const& matrix) {
	Matrix<n, m, T> result = Matrix<n, m, T>(coeff);
	for (uint8_t i = 0; i < n * m; i++) {
		result.coeff[i] += coeff[i] + matrix->coeff[i];
	}
	return result;
}

template<uint8_t n, uint8_t m, typename T>
void Matrix<n, m, T>::operator += (Matrix* const& matrix) {
	for (uint8_t i = 0; i < n * m; i++) {
		coeff[i] += matrix->coeff[i];
	}
}

template<uint8_t n, uint8_t m, typename T>
template<uint8_t b>
Matrix<n,b,T> Matrix<n,m,T>::operator * (Matrix<m,b,T>* const& matrix) {
	Matrix<n, b, T> result = Matrix<n, b, T>(nullptr);
	for (uint8_t i = 0; i < n; i++) {
		for (uint8_t j = 0; j < b; j++) {
			T sum = 0;
			for (uint8_t cur = 0; cur < m; cur++) {
				sum += coeff[m * i + cur] * matrix->coeff[j + cur*b];
			};
			result.coeff[i * b + j] = sum;
		};
	}
	return result;
}






//Functions

template<typename T, typename T1>
T lerp(T source, T target, T1 percent) {
	percent = Clamp<T1>(percent, (T1)0.0, (T1)1.0);
	return source + (target - source) * percent;
}

template<typename T, typename T1>
Vector2<T> lerpVector2(Vector2<T> source, Vector2<T> target, Vector2<T1> percent) {
	return Vector2<T>(lerp(source.x, target.x, percent.x), lerp(source.y, target.y, percent.y));
};

template<typename T, typename T1>
Vector3<T> lerpVector3(Vector3<T> source, Vector3<T> target, Vector3<T1> percent) {
	return Vector3<T>(lerp(source.x, target.x, percent.x), lerp(source.y, target.y, percent.y), 
					  lerp(source.z, target.z, percent.z));
};
//Quadratic Bezier
template<typename T, typename T1>
T Qbezier(T source, T target, T point, T1 percent) {
	percent = Clamp<T1>(percent, (T1)0.0, (T1)1.0);
	T1 t2 = percent * percent;
	T1 x = 1 - percent;
	T1 x2 = x * x;

	return x2 * source + 2 * x * percent * point + t2 * target;
}

template<typename T, typename T1>
Vector2<T> QbezierVector2(Vector2<T> source, Vector2<T> target, Vector2<T> point, Vector2<T1> percent) {
	return Vector2<T>(Qbezier(source.x, target.x, point.x, percent.x),
					  Qbezier(source.y, target.y, point.y, percent.y));
	//DevNote: Should maybe rewrite code for each so it's optimized, look at how many time calculation is redone
}

template<typename T, typename T1>
Vector3<T> QbezierVector3(Vector3<T> source, Vector3<T> target, Vector3<T> point, Vector3<T1> percent) {
	return Vector3<T>(Qbezier(source.x, target.x, point.x, percent.x),
					  Qbezier(source.y, target.y, point.y, percent.y),
					  Qbezier(source.z, target.z, point.z, percent.z));
}

//Cubic bezier
template<typename T, typename T1> 
T Cbezier(T source, T target, T point1, T point2, T1 percent) {
	percent = Clamp<T1>(percent, (T1)0.0, (T1)1.0);
	T1 t2 = percent * percent;
	T1 t3 = percent * t2;
	T1 x = 1 - percent;
	T1 x2 = x * x;
	T1 x3 = x2 * x;

	return  x3 * source + 3 * percent * x2 * point1 + 3 * t2 * x * point2 + x3 * target;
}

template<typename T, typename T1>
Vector2<T> CbezierVector2(Vector2<T> source, Vector2<T> target, Vector2<T> point1, Vector2<T> point2, Vector2<T1> percent) {
	return Vector2<T>(Cbezier(source.x, target.x, point1.x, point2.x, percent.x),
					  Cbezier(source.y, target.y, point1.y, point2.y, percent.y));
	//DevNote: Should maybe rewrite code for each so it's optimized, look at how many time calculation is redone
}

template<typename T, typename T1>
Vector3<T> CbezierVector3(Vector3<T> source, Vector3<T> target, Vector2<T> point1, Vector2<T> point2, Vector3<T1> percent) {
	return Vector3<T>(Cbezier(source.x, target.x, point1.x, point2.x, percent.x),
					  Cbezier(source.y, target.y, point1.y, point2.y, percent.y),
					  Cbezier(source.z, target.z, point1.z, point2.z, percent.z));
}




template<typename T>
T Det2(Vector2<T> a, Vector2<T> b) {
	return a.x * b.y - a.y * b.x;
}