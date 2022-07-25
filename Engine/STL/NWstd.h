#pragma once
#include "Globals.h"
//NWengine standard library
template<int s>
class pArray {
private:
	int topPtr = 0;
public:
	void* ptrArray[s+1] = {0};  //array of pointers
	int size;
	pArray() { size = s; };
	~pArray();
	template<typename T>
	void Add(T element);
	template<typename T>
	void Pop(int index = -1);
	template<typename T>
	T Get(int index);
	template<typename T>
	T* GetPtr(int index);
};


//TODO::Complete the tree code
template<typename T, int n>
class GeneralTreeNode {

public:
	T value;
	int nextNum; //Number of branches from here
	GeneralTreeNode* arr[n];
	GeneralTreeNode* operator[] (int const& index);
	GeneralTreeNode();
};


template<typename T, int n>
class GeneralTree {
	GeneralTreeNode<T,n> head;
	GeneralTree();
};


inline std::string stringMul(std::string str, uint8 mul) {
	std::string result = "";
	for (uint8 i = 0; i < mul; i++) {
		result += str; //Very basic and not optimized?
	}
	return result;
}