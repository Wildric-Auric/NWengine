#ifndef MATHS_H
#define MATHS_H
//TODO::Add substraction and division
template<class T>
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
			
			template<class T>
			bool Vector2<T>::operator == (Vector2 const& vec1) {
				return (x == vec1.x) && (y == vec1.y);
			}




template<typename T> 
int sign(T number) {
	if (number < 0) return -1;
		return 1;
}


template<class T>
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
			template<class T>
			Vector3<T> Vector3<T>::operator + (Vector3 const& vec1) {
				Vector3 sum;
				sum.x = x + vec1.x;
				sum.y = y + vec1.y;
				sum.z = z + vec1.z;
				return sum;
			}
			template<class T>
			Vector3<T> Vector3<T>::operator * (Vector3 const& vec1) {
				Vector3 newVec;
				newVec.x = x * vec1.x;
				newVec.y = y * vec1.y;
				newVec.z = z * vec1.z;
				return newVec;
			}
			template<class T>
			Vector3<T> Vector3<T>::operator + (T const& num) {
				Vector3 newVec;
				newVec.x = x + num;
				newVec.y = y + num;
				newVec.z = z + num;
				return newVec;
			}

			template<class T>
			Vector3<T> Vector3<T>::operator * (T const& num) {
				Vector3 newVec;
				newVec.x = x * num;
				newVec.y = y * num;
				newVec.z = z * num;
				return newVec;
			}
			template<class T>
			bool Vector3<T>::operator == (Vector3 const& vec1) {
				return (x == vec1.x) && (y == vec1.y) && (z == vec1.z);
			}

#endif