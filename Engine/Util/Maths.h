#pragma once
#include "stdf.h"

#define UI8 unsigned char
#pragma warning(disable: 4244) 

#define PI 3.14159265359
#define ONEDIV180 0.00555555555

inline float DegToRad(float degree) {
	return degree * PI * ONEDIV180;
}

inline float RadToDeg(float rad) {
	return 180.0 * rad / PI;
}



template<typename T>
class Vector2 {
public:
	struct {
		T x;
		T y;
	};
	Vector2(T x = 0.0f, T y = 0.0f);

	Vector2<float> normalize();
	float magnitude();
	T Dot(Vector2 const& vec1);
	Vector2 Project(Vector2 const& vec1);
	Vector2 Rotate(float const& angle); //In degree

	Vector2 operator + (Vector2 const& vec1) const;
	Vector2 operator + (T const& num)        const;
	Vector2 operator - (Vector2 const& vec1) const;
	Vector2 operator - (T const& num)        const;
	template<typename T1>
	Vector2 operator * (T1 const& num)       const;
	Vector2 operator * (Vector2 const& vec1) const;
	template<typename T1>
	Vector2 operator / (T1 const& num)       const;
	Vector2 operator / (Vector2 const& vec1) const;
    Vector2 operator - ()                    const;
	bool operator == (Vector2 const& vec1)   const;
	bool operator != (Vector2 const& vec1)   const;
	
	operator Vector2<int>() const { return Vector2<int>(x,y); }
	operator Vector2<float>() const { return Vector2<float>(x, y); }

	T* operator [] (int index);
};

template<typename T>
Vector2<T> Vector2<T>::operator - () const {
    return Vector2<T>(-x, -y);
}

template<typename T,typename T1>
Vector2<T> operator * (T1 const& num, Vector2<T> vec) {
    return vec * num;
}

template<typename T>
Vector2<float> Vector2<T>::normalize() {
	float magnitude = NWpow(x * x + y * y, 0.5);
	if (magnitude == 0) {
		return Vector2<float>(0, 0);
	}
	return Vector2<float>(x / magnitude, y / magnitude);
}
template<typename T>
float Vector2<T>::magnitude() {
	return NWpow(x * x + y * y, 0.5);
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
	return Vector2<T>(NWcos(angle0) * x - NWsin(angle0) * y, 
					  NWsin(angle0) * x + NWcos(angle0) * y);
}
template<typename T>
Vector2<T>::Vector2(T x, T y) {
	Vector2::x = x;
	Vector2::y = y;
}
template<typename T>
Vector2<T> Vector2<T>::operator + (Vector2 const& vec1) const {
	Vector2 sum;
	sum.x = x + vec1.x;
	sum.y = y + vec1.y;
	return sum;
}
template<typename T>
Vector2<T> Vector2<T>::operator - (Vector2 const& vec1) const {
	Vector2 sub;
	sub.x = x - vec1.x;
	sub.y = y - vec1.y;
	return sub;
}

template<class T>
Vector2<T> Vector2<T>::operator * (Vector2 const& vec1) const {
	Vector2 newVec;
	newVec.x = x * vec1.x;
	newVec.y = y * vec1.y;
	return newVec;
}
template<typename T> 
Vector2<T> Vector2<T>::operator + (T const& num) const {
	Vector2 newVec;
	newVec.x = x + num;
	newVec.y = y + num;
	return newVec;
}
template<typename T>
Vector2<T> Vector2<T>::operator - (T const& num) const {
	Vector2 newVec;
	newVec.x = x - num;
	newVec.y = y - num;
	return newVec;
}

template<typename T>
template<typename T1>
Vector2<T> Vector2<T>::operator * (T1 const& num) const {
	Vector2 newVec;
	newVec.x = x * num;
	newVec.y = y * num;
	return newVec;
}

template<typename T>
template<typename T1>
Vector2<T> Vector2<T>::operator / (T1 const& num) const {
	Vector2 newVec;
	newVec.x = x / num;
	newVec.y = y / num;
	return newVec;
}

template<class T>
Vector2<T> Vector2<T>::operator / (Vector2 const& vec1) const {
	Vector2 newVec;
	newVec.x = x / vec1.x;
	newVec.y = y / vec1.y;
	return newVec;
}
			
template<typename T>
bool Vector2<T>::operator == (Vector2 const& vec1) const {
	return (x == vec1.x) && (y == vec1.y);
}

template<typename T>
bool Vector2<T>::operator != (Vector2 const& vec1) const {
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
	union {
		struct {
			T x;
			T y;
			T z;
		};

		struct {
			T r;
			T g;
			T b;
		};
	};
	Vector3(T x = 0.0f, T y = 0.0f, T z = 0.0f);

	Vector3<float> normalize() const;
	float magnitude();
	T Dot(Vector3 const& vec1);
    Vector3<T> Cross(Vector3 const&);

	Vector3 Project(Vector3 const& vec1);

	Vector3 operator + (Vector3 const& vec1) const;
	Vector3 operator + (T const& num)		 const; 
    Vector3 operator - (Vector3 const& vec1) const;
    Vector3 operator - (T const& num)        const;
	Vector3 operator * (T const& num)        const; 
	Vector3 operator * (Vector3 const& vec1) const;
	Vector3 operator / (T const& num)        const; 
	Vector3 operator / (Vector3 const& vec1) const;
	Vector3 operator - ()                    const;
	bool operator == (Vector3 const& vec1)   const;
	bool operator != (Vector3 const& vec1)   const;
};

template<typename T>
Vector3<T> Vector3<T>::operator - () const {
   return Vector3<T>(-x, -y, -z);
}

template<typename T>
Vector3<T> operator * (T const& num, Vector3<T> vec) {
    return vec * num;
}

template<typename T>
Vector3<T> Vector3<T>::operator / (T const& num) const {
    return Vector3<T>(this->x/num, this->y/num, this->z/num); 
}

template<typename T>
Vector3<T> Vector3<T>::operator / (Vector3 const& vec1) const {
    return Vector3<T>(x/vec1.x, y/vec1.y, z/vec1.z); 
}

template<typename T>
Vector3<float> Vector3<T>::normalize() const {
	float magnitude = NWpow(x * x + y * y + z * z, 0.5);
	if (magnitude == 0) {
		return Vector3<float>(x,y,z);
	}
	return Vector3<float>(x / magnitude, y / magnitude, z / magnitude);
}
template<typename T>
float Vector3<T>::magnitude() {
	return NWpow(x * x + y * y + z * z,0.5);
}
template<typename T>
T Vector3<T>::Dot(Vector3 const& vec1) {
	return x * vec1.x + y * vec1.y + z * vec1.z;
};

template<typename T> 
Vector3<T> Vector3<T>::Cross(Vector3 const& vec1) {
    return {
      y * vec1.z - z * vec1.y,
      z * vec1.x - x * vec1.z,
      x * vec1.y - y * vec1.x
    };
}

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
Vector3<T> Vector3<T>::operator + (Vector3 const& vec1) const {
	Vector3 sum;
	sum.x = x + vec1.x;
	sum.y = y + vec1.y;
	sum.z = z + vec1.z;
	return sum;
}
template<typename T>
Vector3<T> Vector3<T>::operator * (Vector3 const& vec1) const {
	Vector3 newVec;
	newVec.x = x * vec1.x;
	newVec.y = y * vec1.y;
	newVec.z = z * vec1.z;
	return newVec;
}
template<typename T>
Vector3<T> Vector3<T>::operator + (T const& num) const {
	Vector3 newVec;
	newVec.x = x + num;
	newVec.y = y + num;
	newVec.z = z + num;
	return newVec;
}

template<typename T>
Vector3<T> Vector3<T>::operator - (T const& num) const {
	Vector3 newVec;
	newVec.x = x - num;
	newVec.y = y - num;
	newVec.z = z - num;
	return newVec;
}

template<typename T>
Vector3<T> Vector3<T>::operator - (Vector3 const& vec1) const {
	Vector3 newVec;
	newVec.x = x - vec1.x;
	newVec.y = y - vec1.y;
	newVec.z = z - vec1.z;
	return newVec;
}

template<typename T>
Vector3<T> Vector3<T>::operator * (T const& num) const {
	Vector3 newVec;
	newVec.x = x * num;
	newVec.y = y * num;
	newVec.z = z * num;
	return newVec;
}
template<typename T>
bool Vector3<T>::operator == (Vector3 const& vec1) const {
	return (x == vec1.x) && (y == vec1.y) && (z == vec1.z);
}

template<typename T>
bool Vector3<T>::operator != (Vector3 const& vec1) const {
	return (x != vec1.x) && (y != vec1.y) && (z != vec1.z);
}

template<typename T>
class Vector4 { //TODO::Complete this class
public:
	union {
		struct {
			T x;
			T y;
			T z;
			T w;
		};

		struct {
			T r;
			T g;
			T b;
			T a;
		};
	};
	Vector4(T x = 0.0f, T y = 0.0f, T z = 0.0f, T w = 0.0f);

};

template<typename T>
Vector4<T>::Vector4(T x, T y, T z, T w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


template<UI8 n, UI8 m, typename T>
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
	template<UI8 b>
	Matrix<n, b, T> operator * (Matrix<m, b, T>* const& matrix);
	//void operator *= (Matrix<m, b, T>* const& matrix); Can you do it without creating another matrix??

};

template<UI8 n, UI8 m, typename T>
Matrix<n, m, T>::Matrix(T* data) {
	dimension.x = n;
	dimension.y = m;
	int size = sizeof(T) * n * m;
	if (data!=nullptr) NWmemcpy(coeff, data, size);
	else {
		for (int i = 0; i < n * m; i++) {
			coeff[i] = (T)0;
		};
	}

};

template<UI8 n, UI8 m, typename T>
Matrix<n, m, T> Matrix<n, m, T>::operator + (T const& num) {
	Matrix<n, m, T> result = Matrix<n, m, T>(coeff);
	for (UI8 i = 0; i < n * m; i++) {
		result.coeff[i] += num;
	};
	return result;
}

template<UI8 n, UI8 m, typename T>
void Matrix<n, m, T>::operator += (T const& num) {
	for (UI8 i = 0; i < n * m; i++) {
		coeff[i] += num;
	};
}

template<UI8 n, UI8 m, typename T>
Matrix<n,m,T> Matrix<n, m, T>::operator * (T const& num) {
	Matrix<n,m,T> result = Matrix<n,m,T>(coeff);
	for (UI8 i = 0; i < n * m; i++) {
		result.coeff[i] *= num;
	};
	return result;
}

template<UI8 n, UI8 m, typename T>
void Matrix<n, m, T>::operator *= (T const& num) {
	for (UI8 i = 0; i < n * m; i++) {
		coeff[i] *= num;
	};
}

template<UI8 n, UI8 m, typename T>
Matrix<n, m, T> Matrix<n, m, T>::operator + (Matrix* const& matrix) {
	Matrix<n, m, T> result = Matrix<n, m, T>(coeff);
	for (UI8 i = 0; i < n * m; i++) {
		result.coeff[i] += coeff[i] + matrix->coeff[i];
	}
	return result;
}

template<UI8 n, UI8 m, typename T>
void Matrix<n, m, T>::operator += (Matrix* const& matrix) {
	for (UI8 i = 0; i < n * m; i++) {
		coeff[i] += matrix->coeff[i];
	}
}

template<UI8 n, UI8 m, typename T>
template<UI8 b>
Matrix<n,b,T> Matrix<n,m,T>::operator * (Matrix<m,b,T>* const& matrix) {
	Matrix<n, b, T> result = Matrix<n, b, T>(nullptr);
	for (UI8 i = 0; i < n; i++) {
		for (UI8 j = 0; j < b; j++) {
			T sum = 0;
			for (UI8 cur = 0; cur < m; cur++) {
				sum += coeff[m * i + cur] * matrix->coeff[j + cur*b];
			};
			result.coeff[i * b + j] = sum;
		};
	}
	return result;
}


enum class MATFLAG {
	NOINIT,
};

template<typename T>
class Matrix2 {
	T values[4]; //Column major
	Matrix2();
	Matrix2(T diag);
	Matrix2(MATFLAG flag);
	Matrix2 operator * (const Matrix2& other);
	void    operator *=(const Matrix2& other);
	T operator ()(int i, int j); //Row major access
};
template<typename T>
Matrix2<T>::Matrix2(T diag) {
	values[0] = diag; values[1] = 0;
	values[2] = 0	; values[3] = diag;
}
template<typename T>
Matrix2<T>::Matrix2() : Matrix2<T>((T)1.0) {};

template<typename T>
Matrix2<T>::Matrix2(MATFLAG flag) {}

template<typename T>
Matrix2<T> Matrix2<T>::operator* (const Matrix2<T>& other) {
	Matrix2<T> result = Matrix2(MATFLAG::NOINIT);
	result.values[0] = this->values[0] * other.values[0] + this->values[1] * other.values[2];
	result.values[1] = this->values[0] * other.values[1] + this->values[1] * other.values[3];
	result.values[2] = this->values[2] * other.values[0] + this->values[3] * other.values[2];
	result.values[3] = this->values[2] * other.values[1] + this->values[3] * other.values[3];
	return result;
}

template<typename T>
void Matrix2<T>::operator *= (const Matrix2<T>& other) {
	Matrix2<T> result = Matrix2(MATFLAG::NOINIT);
	result.values[0] = other.values[0] * values[0] + other.values[1] * values[2];
	result.values[1] = other.values[0] * values[1] + other.values[1] * values[3];
	result.values[2] = other.values[2] * values[0] + other.values[3] * values[2];
	result.values[3] = other.values[2] * values[1] + other.values[3] * values[3];
	NWmemcpy(this->values, result.values, 4 * sizeof(T));
}

template<typename T>
T Matrix2<T>::operator()(int i, int j) {
	return j * 2  + i;
}

template<typename T>
class Matrix3 {
	T values[9]; //Column major
	Matrix3();
	Matrix3(T diag);
	Matrix3(MATFLAG flag);
	Matrix3 operator * (const Matrix3& other);
	void    operator *=(const Matrix3& other);
	T       operator ()(int i, int j);
};

template<typename T>
Matrix3<T>::Matrix3(T diag) {
	values[0] = diag; values[1] = 0   ; values[2] = 0;
	values[3] = 0   ; values[4] = diag; values[5] = 0;
	values[6] = 0   ; values[7] = 0   ; values[8] = diag;
}

template<typename T>
Matrix3<T>::Matrix3() : Matrix3<T>((T)1.0) {};

template<typename T>
Matrix3<T>::Matrix3(MATFLAG flag) {}

template<typename T>
Matrix3<T> Matrix3<T>::operator* (const Matrix3<T>&other) {
	Matrix3<T> result = Matrix3(MATFLAG::NOINIT);
	result.values[0] = other.values[0] * values[0] + other.values[1] * values[3] + other.values[2] * values[6];
	result.values[1] = other.values[0] * values[1] + other.values[1] * values[4] + other.values[2] * values[7];
	result.values[2] = other.values[0] * values[2] + other.values[1] * values[5] + other.values[2] * values[8];
	result.values[3] = other.values[3] * values[0] + other.values[4] * values[3] + other.values[5] * values[6];
	result.values[4] = other.values[3] * values[1] + other.values[4] * values[4] + other.values[5] * values[7];
	result.values[5] = other.values[3] * values[2] + other.values[4] * values[5] + other.values[5] * values[8];
	result.values[6] = other.values[6] * values[0] + other.values[7] * values[3] + other.values[8] * values[6];
	result.values[7] = other.values[6] * values[1] + other.values[7] * values[4] + other.values[8] * values[7];
	result.values[8] = other.values[6] * values[2] + other.values[7] * values[5] + other.values[8] * values[8];
	return result;
}

template<typename T>
void Matrix3<T>::operator *= (const Matrix3<T>& other) {
	Matrix3<T> result = Matrix3<T>(MATFLAG::NOINIT);
	result.values[0] = other.values[0] * values[0] + other.values[1] * values[3] + other.values[2] * values[6];
	result.values[1] = other.values[0] * values[1] + other.values[1] * values[4] + other.values[2] * values[7];
	result.values[2] = other.values[0] * values[2] + other.values[1] * values[5] + other.values[2] * values[8];
	result.values[3] = other.values[3] * values[0] + other.values[4] * values[3] + other.values[5] * values[6];
	result.values[4] = other.values[3] * values[1] + other.values[4] * values[4] + other.values[5] * values[7];
	result.values[5] = other.values[3] * values[2] + other.values[4] * values[5] + other.values[5] * values[8];
	result.values[6] = other.values[6] * values[0] + other.values[7] * values[3] + other.values[8] * values[6];
	result.values[7] = other.values[6] * values[1] + other.values[7] * values[4] + other.values[8] * values[7];
	result.values[8] = other.values[6] * values[2] + other.values[7] * values[5] + other.values[8] * values[8];
	NWmemcpy(this->values, result.values, 4 * sizeof(T));
}

template<typename T>
T Matrix3<T>::operator()(int i, int j) {
	return j * 3 + i;
}

//4x4--------------------------
template<typename T>
class Matrix4 { public: T values[16]; //Column major
	Matrix4();
	Matrix4(T diag);
	Matrix4(MATFLAG flag);
	Matrix4 operator * (const Matrix4& other);
	Vector4<T> operator * (const Vector4<T>& other);
	void    operator *=(const Matrix4& other);
	T operator ()(int i, int j); //Row major access
};

template<typename T>
Matrix4<T>::Matrix4(T diag) {
	values[0] = diag; values[1] = 0;    values[2] = 0;		values[3] = 0;
	values[4] = 0;    values[5] = diag; values[6] = 0;		values[7] = 0;
	values[8] = 0;    values[9] = 0;    values[10] = diag;   values[11] = 0;
	values[12] = 0;   values[13] = 0;   values[14] = 0;		values[15] = diag;
}
template<typename T>
Matrix4<T>::Matrix4() : Matrix4<T>((T)1.0) {};

template<typename T>
Matrix4<T>::Matrix4(MATFLAG flag) {}

template<typename T>
Matrix4<T> Matrix4<T>::operator* (const Matrix4<T>& other) {
	Matrix4<T> result = Matrix4(MATFLAG::NOINIT);
#define TMP_MAT_4x4_MUL \
	result.values[0] = other.values[0]   * values[0] + other.values[1]  * values[4] + other.values[2]  *  values[8] +  other.values[3] *  values[12];     \
	result.values[1] = other.values[0]   * values[1] + other.values[1]  * values[5] + other.values[2]  *  values[9] +  other.values[3] * values[13];	  \
	result.values[2] = other.values[0]   * values[2] + other.values[1]  * values[6] + other.values[2]  *  values[10] + other.values[3] *  values[14];	  \
	result.values[3] = other.values[0]   * values[3] + other.values[1]  * values[7] + other.values[2]  *  values[11] + other.values[3] *  values[15];	  \
	result.values[4] = other.values[4]   * values[0] + other.values[5]  * values[4] + other.values[6]  *  values[8] +  other.values[7] * values[12];	  \
	result.values[5] = other.values[4]   * values[1] + other.values[5]  * values[5] + other.values[6]  *  values[9] +  other.values[7] * values[13];	  \
	result.values[6] = other.values[4]   * values[2] + other.values[5]  * values[6] + other.values[6]  *  values[10] + other.values[7] *  values[14];	  \
	result.values[7] = other.values[4]   * values[3] + other.values[5]  * values[7] + other.values[6]  *  values[11] + other.values[7] *  values[15];	  \
	result.values[8] = other.values[8]   * values[0] + other.values[9]  * values[4] + other.values[10] * values[8] +  other.values[11] * values[12];	  \
	result.values[9] = other.values[8]   * values[1] + other.values[9]  * values[5] + other.values[10] * values[9] +  other.values[11] * values[13];	  \
	result.values[10] = other.values[8]  * values[2] + other.values[9]  * values[6] + other.values[10] * values[10] + other.values[11] * values[14];	  \
	result.values[11] = other.values[8]  * values[3] + other.values[9]  * values[7] + other.values[10] * values[11] + other.values[11] * values[15];	  \
	result.values[12] = other.values[12] * values[0] + other.values[13] * values[4] + other.values[14] * values[8] +  other.values[15] * values[12];	  \
	result.values[13] = other.values[12] * values[1] + other.values[13] * values[5] + other.values[14] * values[9] +  other.values[15] * values[13];	  \
	result.values[14] = other.values[12] * values[2] + other.values[13] * values[6] + other.values[14] * values[10] + other.values[15] * values[14];	  \
	result.values[15] = other.values[12] * values[3] + other.values[13] * values[7] + other.values[14] * values[11] + other.values[15] * values[15];	  

	TMP_MAT_4x4_MUL
	return result;
}

template<typename T>
void Matrix4<T>::operator *= (const Matrix4<T>& other) {
	Matrix4<T> result = Matrix4(MATFLAG::NOINIT);
	TMP_MAT_4x4_MUL
	NWmemcpy(this->values, result.values, 4 * sizeof(T));
}
#undef TMP_MAT_4x4_MUL

template<typename T>
Vector4<T> Matrix4<T>::operator * (const Vector4<T>& other) {
	Vector4<T> result;
	result.x = values[0] * other.x + values[4] * other.y + values[8] * other.z + values[12] * other.w;
	result.y = values[1] * other.x + values[5] * other.y + values[9] * other.z + values[13] * other.w;
	result.z = values[2] * other.x + values[6] * other.y + values[10] * other.z + values[14] * other.w;
	result.w = values[3] * other.x + values[7] * other.y + values[11] * other.z + values[15] * other.w;
	return result;
}



template<typename T>
T Matrix4<T>::operator()(int i, int j) {
	return j * 4 + i;
}

//Matrix passed as reference to this function should be identity matrix; otherwise, behaviour is undefined
inline void OrthorgraphicMat(Matrix4<float>& matrix, float left, float right, float buttom, float top, float near = 1.0f, float far = -1.0f) {
	float rml = 1.0f / (right - left);
	float tmb = 1.0f / (top   - buttom);
	float fmn = 1.0f / (far   - near);
	matrix.values[0]   =  2.0f * rml;
	matrix.values[5]   =  2.0f * tmb;
	matrix.values[10]  = -2.0f * fmn;

	matrix.values[12] = (-right - left)  * rml;
	matrix.values[13] = (- top  - buttom)* tmb;
	matrix.values[14] = (- far  - near)  * fmn;
}
//Matrix passed as reference to this function should be identity matrix
inline void PerspectiveMat(Matrix4<float>& matrix, float degreeFovY,float aspect, float near, float far) {
    float cotn  =     1.0f / NWtan(DegToRad(degreeFovY) / 2.0f);
    float fmn   =     1.0f / (near - far);

    matrix.values[0]   = (1.0f / aspect) * cotn;
    matrix.values[5]   = cotn;
    matrix.values[10]  = (far + near) * fmn;
    matrix.values[11]  = -1.0f; //Left handed
    matrix.values[14]  = 2.0f*far*near*fmn;
    matrix.values[15]  = 0.0f;
}
//TODO::Add perspective matrix that takes width and height as args

template<typename T> 
void LookAt(Matrix4<T>&matrix, const Vector3<T>& camPos, const Vector3<T>& targetPos, const Vector3<T>& upVec) { 
	//TODO::Test this
	//TODO::const version
	Vector3<T> up	  = upVec;
	Vector3<T> target = targetPos;
	Vector3<T> cameraPosition = camPos;
    Vector3<T> dir   = (cameraPosition - target).normalize();
  
    Vector3<T> right  = up.Cross(dir);
    Vector3<T> u      = dir.Cross(right);

    matrix.values[0]  = right.x;
    matrix.values[1]  = u.x;
    matrix.values[2]  = dir.x;

    matrix.values[4]  = right.y;
    matrix.values[5]  = u.y ;
    matrix.values[6]  = dir.y;

    matrix.values[8]  = right.z ;
    matrix.values[9]  = u.z;
    matrix.values[10] = dir.z ;

    matrix.values[12] =  -right.Dot(camPos);
    matrix.values[13]  = -u.Dot(camPos);
    matrix.values[14]  = -dir.Dot(camPos);
}

template<typename T>
void TranslateMat(Matrix4<T>&matrix, const Vector3<T>& coeff) {
	matrix.values[12]   += coeff.x;
	matrix.values[13]	+= coeff.y;
	matrix.values[14]   += coeff.z;
}

template<typename T>
void ScaleMat(Matrix4<T>& matrix, const Vector3<T>& coeff) {
	matrix.values[0]  *= coeff.x ;
	matrix.values[5]  *= coeff.y;
	matrix.values[10] *= coeff.z;
}

//TODO::Optimize this and do it in palce
template<typename T>
void RotateMat(Matrix4<T>& matrix, float degAngle, const Vector3<T>& axis) {
	Matrix4<T> newMat(MATFLAG::NOINIT);
	degAngle	   = DegToRad(degAngle);
	float   c	   = NWcos(degAngle);
	float   cmpC   = 1.0f - c;  // NAME: Complementary of cos
	float   s      = NWsin(degAngle);

	Vector3<float> n = axis.normalize();

	newMat.values[0] = c + n.x * n.x * cmpC;
	newMat.values[1] = cmpC * n.x * n.y + n.z * s;
	newMat.values[2] = cmpC * n.z * n.x - n.y * s;
	newMat.values[3] = 0;

	newMat.values[4] = cmpC * n.x * n.y - n.z * s;
	newMat.values[5] = c    + n.y * n.y * cmpC;
	newMat.values[6] = n.y * n.z * cmpC + n.x * s;
	newMat.values[7] = 0;

	newMat.values[8]  = n.x * n.z * cmpC + n.y * s;
	newMat.values[9]  = n.y * n.z * cmpC - n.x * s;
	newMat.values[10] = c   + n.z * n.z * cmpC;
	newMat.values[11] = 0;

	newMat.values[12] = 0;
	newMat.values[13] = 0;
	newMat.values[14] = 0;
	newMat.values[15] = 1;

	matrix = newMat * matrix;
}


//Functions---------------------------------------

template<typename T, typename T1>
T lerp(T source, T target, T1 percent) {
	percent = Clamp<T1>(percent, (T1)0.0, (T1)1.0);
	return source + (target - source) * percent;
}

template<typename T, typename T1> Vector2<T> lerpVector2(const Vector2<T>& source, const Vector2<T>& target, const Vector2<T1>& percent) {
	return Vector2<T>(lerp(source.x, target.x, percent.x), lerp(source.y, target.y, percent.y));
};

template<typename T, typename T1>
Vector3<T> lerpVector3(const Vector3<T>& source, const Vector3<T>& target, const Vector3<T1>& percent) {
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
Vector2<T> QbezierVector2(const Vector2<T>& source, const Vector2<T>& target, const Vector2<T>& point, const Vector2<T1>& percent) {
	return Vector2<T>(Qbezier(source.x, target.x, point.x, percent.x),
					  Qbezier(source.y, target.y, point.y, percent.y));
	//DevNote: Should maybe rewrite code for each so it's optimized, look at how many time calculation is redone
}

template<typename T, typename T1>
Vector3<T> QbezierVector3(const Vector3<T>& source, const Vector3<T>& target, const Vector3<T>& point, const Vector3<T1>& percent) {
	return Vector3<T>(Qbezier(source.x, target.x, point.x, percent.x),
					  Qbezier(source.y, target.y, point.y, percent.y),
					  Qbezier(source.z, target.z, point.z, percent.z));
}

//Cubic bezier
template<typename T, typename T1> 
T Cbezier(T source, T target, T point1, T point2, T1 percent) {
	percent = Clamp<T1>(percent, (T1)0.0, (T1)1.0);
	T1 t2 = percent * percent;
	T1 t3 = percent * t2;       //t3
	T1 x = 1 - percent;  
	T1 x2 = x * x;        //(1-t)^2
	T1 x3 = x2 * x;       //(1-t)^3

	return  x3 * source + 3 * percent * x2 * point1 + 3 * t2 * x * point2 + t3 * target;
}

template<typename T, typename T1>
Vector2<T> CbezierVector2(const Vector2<T>& source, const Vector2<T>& target, const Vector2<T>& point1, const Vector2<T>& point2, const Vector2<T1>& percent) {
	return Vector2<T>(Cbezier(source.x, target.x, point1.x, point2.x, percent.x),
					  Cbezier(source.y, target.y, point1.y, point2.y, percent.y));
	//DevNote: Should maybe rewrite code for each so it's optimized, look at how many time calculation is redone
}

template<typename T, typename T1>
Vector3<T> CbezierVector3(const Vector3<T>& source, const Vector3<T>& target, const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T1>& percent) {
	return Vector3<T>(Cbezier(source.x, target.x, point1.x, point2.x, percent.x),
					  Cbezier(source.y, target.y, point1.y, point2.y, percent.y),
					  Cbezier(source.z, target.z, point1.z, point2.z, percent.z));
}

template<typename T>
T Det2(const Vector2<T>& a, const Vector2<T>& b) {
	return a.x * b.y - a.y * b.x;
}

#undef UI8 
