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