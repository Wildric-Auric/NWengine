#pragma once
//TODO::Add substraction and division
//TODO::Add direct multiplication and addition operator
#include <iostream>


template<typename T>
class Vector2 {
public:
	T x;
	T y;
	Vector2(T x = 0.0f, T y = 0.0f);

	Vector2<float> normalize();
	float magnitude();
	Vector2 operator + (Vector2 const& vec1);
	Vector2 operator + (T const& num);
	template<typename T1>
	Vector2 operator * (T1 const& num);
	Vector2 operator * (Vector2 const& vec1);
	bool operator == (Vector2 const& vec1);
	T* operator [] (int index);
};


template<typename T>
Vector2<float> Vector2<T>::normalize() {
	float magnitude = pow(x * x + y * y, 0.5);
	if (magnitude == 0) {
		printf("What the fuck?");
		return Vector2<float>(0, 0);
	}
	return Vector2<float>(x / magnitude, y / magnitude);
}
template<typename T>
float Vector2<T>::magnitude() {
	return pow(x * x + y * y, 0.5);
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
T* Vector2<T>::operator [] (int index) {
	return  index == 0 ? &x : &y;
}



template<typename T> 
int sign(T number) {
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

	Vector3 operator + (Vector3 const& vec1);
	Vector3 operator + (T const& num);
	Vector3 operator * (T const& num);
	Vector3 operator * (Vector3 const& vec1);
	bool operator == (Vector3 const& vec1);
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




template<typename T, typename T1>
T lerp(T source, T target, T1 percent) {
	return source + (target - source) * percent;
}

template<typename T, typename T1>
Vector2<T> lerpVector2(Vector2<T> source, Vector2<T> target, Vector2<T1> percent) {
	return Vector2<T>(  lerp(source.x, target.x, percent.x), lerp(source.y, target.y, percent.y)  );
};

template<typename T, typename T1>
Vector3<T> lerpVector3(Vector3<T> source, Vector3<T> target, Vector3<T1> percent) {
	return Vector3<T>(lerp(source.x, target.x, percent.x), lerp(source.y, target.y, percent.y));
};



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

