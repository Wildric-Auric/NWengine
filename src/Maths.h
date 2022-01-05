#pragma once
//TODO::Add substraction and division
#include <iostream>

template<typename T>
class Vector2 {
public:
	T x;
	T y;
	Vector2(T x = 0.0f, T y = 0.0f);
	Vector2 operator + (Vector2 const& vec1);
	Vector2 operator + (T const& num);
	Vector2 operator * (T const& num);
	Vector2 operator * (Vector2 const& vec1);
	bool operator == (Vector2 const& vec1);
};



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
			Vector2<T> Vector2<T>::operator * (T const& num) {
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
int sign(T number) {
	if (number < 0) return -1;
		return 1;
}


template<typename T>
class Vector3 {
public:
	T x;
	T y;
	T z;
	Vector3(T x = 0.0f, T y = 0.0f, T z = 0.0f);
	Vector3 operator + (Vector3 const& vec1);
	Vector3 operator + (T const& num);
	Vector3 operator * (T const& num);
	Vector3 operator * (Vector3 const& vec1);
	bool operator == (Vector3 const& vec1);
};

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
	return Vector2<T>(  lerp(source.x, target.x, percent.x), 
						lerp(source.y, target.y, percent.y), 
						lerp(source.z, target.z, percent.z) );
};

template<typename T>
class Matrix2 {
	//You can pass a new parameter to the array thanks to template but it will be kept simpler here for optimization
public:
	T coeff[2][2];
	Matrix2(T* setOfData);
};

template<typename T>
		Matrix2<T>::Matrix2(T* setOfData) {
			coeff[0][0] = setOfData[0];
			coeff[0][1] = setOfData[1];
			coeff[1][0] = setOfData[2];
			coeff[1][1] = setOfData[3];
			std::cout << coeff[0][0]<< "  "<< coeff[0][1]<< std::endl;
			std::cout << coeff[1][0] << "  " << coeff[1][1] << std::endl;
		}