#include "NWstd.h"


template<int s>
template<typename T>
void pArray<s>::Add(T element) {
	T* elmentPtr = new T;
	*elementPtr = element;
	arr[topPtr] = (long) elementPtr;
	topPtr += 1;
};


template<int s>
template<typename T>
void pArray<s>::Pop(int index) {
	if (index == -1) {
		index = this->topPtr;
		delete (T*)ptrArray[topPtr];
		ptrArray[topPtr] = 0;
	}

	else {
		delete (T*)ptrArray[index];
		ptrArray[index] = 0;
		//TODO::Do it without a loop, it's slow maybe
		for (int i = index; i < size ;i++) {
			if (ptrArray[i + 1] == 0) {
				ptrArray[i] = 0;
				break;
			}
			ptrArray[i] = ptrArray[i + 1];
		}
	}
	topPtr -= 1;
};


template<int s>
template<typename T>
T pArray<s>::Get(int index) {
	return *((T*)ptrArray[index]);
}


template<int size>
template<typename T>
T* pArray<size>::GetPtr(int index) {
	return (T*)ptrArray[index];
}

template<int s>
pArray<s>::~pArray() {
	//Find how to delete data of all addresses
};