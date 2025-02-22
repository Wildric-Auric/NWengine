#pragma once
/*
   This is a custom implementation of datastructures.
   It was written to enforce memory locality, and get rid
   of template instanciation overhead.
   It is experimental and DOES NOT support storing classes having constructors and destructors.
*/


#include "dsTypes.h"

enum class AllocStrat {
    NONE,
    EQUAL,
    DOUBLE
};

class MemoryRegion {
public: 
    ui32 _size           = 0;
    ui32 _sp             = 0;
    ui32 _unitSizeInByte = 0;
    addr _data           = 0;
    MemoryRegion* _next = 0;
    MemoryRegion* _prev = 0; 
    AllocStrat _subOptMemStrat = AllocStrat::NONE;
    
    void SetUp(ui32 unitSize, ui32 size);
    void SetUp(ui32 unitSize, ui32 size, const AllocStrat&);
    void SetAllocationStrat(const AllocStrat&);
    void Clean();
    addr Alloc(); 
    void Dealloc(addr ptr);
    addr Get(ui32);
    bool IsValid();
    void CopyFrom(MemoryRegion&);
    bool IsCompatible(MemoryRegion&);
    bool IsSubOpt();

    ui32 _PopStack();
    void _PushStack(ui32 index);
    void _AttachNext();
    void _CleanNext();
    
    ui64 _GetIndexFromAddr(addr);
};


class Deque {
    public:
    void SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx);
    void SetUp(ui32 unitSize, ui32 cap);
    void PushFront(void*);
    void PushBack(void*);
    void PopFront();
    void PopBack();
    addr GetFront();
    addr GetBack();
    addr Get(ui32);
    
    MemoryRegion _ownedRgn;
    MemoryRegion* rgn;
    ui32 _cap = 0;
    ui32 _size = 0;
    ui32 _front;
    ui32 _back;

    //Generics
    template<typename T> 
    void tPushBack(const T& v) {
        PushBack((void*)&v);
    }

    template<typename T> 
    void tPushFront(const T& v) {
        PushFront((void*)&v);
    }

    template<typename T>
    T* tGetFront() {
        return (T*)GetFront();
    }

    template<typename T>
    T* tGetBack() {
        return (T*)GetBack();
    }

    template<typename T>
    T* tGet(ui32 idx) {
        return (T*)Get(idx);
    }
};

class Stack {
    public:
    void SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx);
    void SetUp(ui32 unitSize, ui32 cap);
    void PushBack(void*);
    void PopBack();
    addr GetBack();
    addr Get(ui32);
    
    MemoryRegion _ownedRgn;
    MemoryRegion* rgn;
    ui32 _cap = 0;
    ui32 _size = 0;
    ui32 _back = 0;
    ui32 _base = 0;

    template<typename T> 
    void tPushBack(const T& v) {
        PushBack((void*)&v);
    }

    template<typename T>
    T* tGetBack() {
        return (T*)GetBack();
    }

    template<typename T>
    T* tGet(ui32 idx) {
        return (T*)Get(idx);
    }
};

#define DLIST_FREE_SLOT -2
#define DLIST_POINT_TO_NONE -1

class DlistNode {
    public:
    void* val;
    ui32 _next = 0;
    ui32 _prev = 0;
};

//Dlist only uses owned memory for now
class DList {
    public:
    DList();
    DList(const DList& other);
        
    //void SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx);
    void SetUp(ui32 unitSize, ui32 cap);

    addr AddFirst(void* element);
    addr AddAfter(addr loc, void* element);
    addr AddLast(void* element);
    addr GetNext(addr); 
    addr DirectGetNext(addr); 
    addr GetPrev(addr); 
    addr GetLast();
    addr GetFirst();
    ui64 GetIdx(addr);
    addr Erase(addr);
    ui32 GetPrevIdx(addr);
    ui32 GetNextIdx(addr);
    void* GetContent(addr);
    addr _AddDetached(void*);
    addr _AddDetachedNode(const DlistNode* n, addr loc);
    void _AttachPrev(addr ad, ui32 p);
    void _AttachNext(addr ad, ui32 p);
    void _SetContent(addr ad, void* v);

    addr IterateAndGet(ui32);

    MemoryRegion _ownedRgn;
    MemoryRegion* rgn;
    ui32 _first= -1;
    ui32 _last = -1;
    ui32 _cap  = 0;
    ui32 _base = 0;
    ui32 _size = 0;

    //Generic functions
    template<typename T> 
    addr tAddAfter(addr loc, const T& v) {
        return AddAfter(loc,(void*)&v);
    }
    template<typename T> 
    addr tAddFirst(const T& v) {
        return AddFirst((void*)&v);
    }
    template<typename T> 
    addr tAddLast(const T& v) {
        return AddLast((void*)&v);
    } 
};


typedef ui64 (*hashProc)(const void*, const ui32 sizeInBytes);

extern ui64 defaultHashFNV1A(const void* data, const ui32 sib);


struct HashData{
    void* key;
    void* data; 
    ui32 processedHash;
};


class HashMap {
    public:
    void SetUp(ui32 keySizeInBytes, ui32 valueSizeInBytes, ui32 collisionCap, ui32 cap);
    addr Get(void* key);
    void* GetValue(void* key);
    addr Set(void* key, void* value);
    addr Add(void* key, void* value);
    bool Erase(void* key);
    void* ReadKey(addr);
    void* ReadValue(addr);
    addr _Add(const HashData&);
    addr _Set(const HashData&);
    bool _Erase(const HashData&);
    ui32 _GetIdxFromKey(void* key);
    ui32 _IncPtr(ui32);
    ui32 _DecPtr(ui32);
    ui32 _GetPtr(ui32);

    MemoryRegion  _ownedRgn;
    MemoryRegion* rgn;

    hashProc hashFunc = defaultHashFNV1A;
    ui32 _keySizeInBytes;
    ui32 _valueSizeInBytes;
    ui32 _collisionCap;


    //Generic functions
    template<typename KeyType, typename ValueType> 
    addr tAdd(const KeyType& k, const ValueType& v) {
        return Add((void*)&k, (void*)&v);
    }
    template<typename KeyType, typename ValueType> 
    addr tSet(const KeyType& k, const ValueType& v) {
        return Set(&k, &v);
    }
    template<typename KeyType, typename ValueType> 
    ValueType* tGetValue(const KeyType& key) {
        return (ValueType*)GetValue((void*)&key);
    }
    template<typename KeyType>
    bool tErase(const KeyType& k) {
        return Erase((void*)&k);
    } 
    template<typename KeyType>
    KeyType* tReadKey(const addr ad) {
        return (KeyType*)ReadKey(ad);
    }
    template<typename ValueType>
    ValueType* tReadValue(const addr ad) {
        return (ValueType*)ReadValue(ad);
    }
};


class String {
    public:
    Stack container;
    void SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx);
    void SetUp(ui32 cap);
    char GetChar(int);
    void Insert(char c); 
    void Concat(String&); 
    void Concat(const char*);
    const char* GetData();
};
