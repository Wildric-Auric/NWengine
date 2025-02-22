#include "ds.h"
#include <stdlib.h>
#include <cstring>

#define CPY(dst, src, type) memcpy(dst, src, sizeof(type))

void MemoryRegion::SetUp(ui32 u, ui32 s) {
    _size             = s;
    _unitSizeInByte   = u;
    _data             = malloc(u * s + s*sizeof(ui32));
    ui32* ptr = (ui32*)_data;
    ui32 i;
    for (i = 0; i < s; ++i)
        *(ptr + s-i-1) = i;    
    _sp = s-1;
}

void MemoryRegion::SetAllocationStrat(const AllocStrat& strat) {
    _subOptMemStrat = strat;
}

void MemoryRegion::SetUp(ui32 unitSize, ui32 size, const AllocStrat& strat) {
    SetUp(unitSize, size);
    SetAllocationStrat(strat);
}

bool MemoryRegion::IsSubOpt() {
    return _next != 0;
}

void MemoryRegion::CopyFrom(MemoryRegion& other) {
    for (int i = 0; i < other._size; ++i) {
        addr adr = Alloc();
        memcpy(adr,other.Get(i), _unitSizeInByte);
    }
    for (int i = 0; i <= other._sp; ++i) {
        Dealloc(Get(0x0 + i));
    }
}

bool MemoryRegion::IsCompatible(MemoryRegion& other) {
    return other._unitSizeInByte == this->_unitSizeInByte;
}

void MemoryRegion::Clean() {
    free(_data);
    _CleanNext();
}

addr MemoryRegion::Alloc() {
    if (_sp == -1) {
        if (_subOptMemStrat == AllocStrat::NONE) {
            return 0; 
        }
        if (_next == 0) {
            _AttachNext();
        }
        return _next->Alloc();
    }
    ui32 index = _PopStack();
    return ((byte*)_data + _size * sizeof(ui32) + index * _unitSizeInByte);
}

void MemoryRegion::Dealloc(addr ad) {
    ui64 index = _GetIndexFromAddr(ad);
    if (index == -1 || index >= (ui64)_size) { 
        //address is supposed valid, and we are supposed using subotimal memory if the latter is not in chunk
        _next->Dealloc(ad);
        return;
    }
    _PushStack(index);
}

void MemoryRegion::_AttachNext() {
    _next = new MemoryRegion();
    _next->_size = _subOptMemStrat == AllocStrat::EQUAL ? _size :
                  (_subOptMemStrat == AllocStrat::DOUBLE ? _size * 2 : _size);
    _next->SetUp(_unitSizeInByte, _next->_size);
    _next->_subOptMemStrat = _subOptMemStrat;
    _next->_prev = this;
}

void MemoryRegion::_CleanNext() {
    if (!_next)
        return;
    _next->Clean();
    delete _next;
    _next = 0;
}

void MemoryRegion::_PushStack(ui32 index) {
    ++_sp;
    *(CAST(ui32*, _data) + _sp) = index; 
}

ui32 MemoryRegion::_PopStack() {
    ui32 val = *(CAST(ui32*, _data) + _sp);
    --_sp;
    return val;
}

addr MemoryRegion::Get(ui32 index) {
    if (index < _size) {
        return ((byte*)_data + _size * sizeof(ui32) + index * _unitSizeInByte); 
    }
    if (_next == nullptr) 
        return 0;
    return _next->Get(index - _size);
}

bool MemoryRegion::IsValid() {
    return _size != 0;
}

ui64 MemoryRegion::_GetIndexFromAddr(addr a) {
    ui64 offset = CAST(ui64,a);
    ui64 base = CAST(ui64,Get(0));
    ui64 diff = (offset - base) / _unitSizeInByte;
    if (offset < base || (diff >= _size) ) {
        if (!_next) return -1;
        ui64 ret = _next->_GetIndexFromAddr(a); 
        return (ret == -1) ? -1 : _size + ret;
    }
    return diff;
}

void Deque::SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx) {
    rgn = memReg;
    _front = endIdx;
    _back = begIdx;
    _cap = endIdx - begIdx + 1;
}

void Deque::SetUp(ui32 unitSize, ui32 cap) {
    rgn = &_ownedRgn;
    _ownedRgn.SetUp(unitSize, cap);
    _front = cap/2 + 1; 
    _back  = cap/2 - 1;
    _cap   = cap; 
}

void Deque::PushFront(void* val) {
    --_front;
    ++_size;
    _front == _back ? ++_back: 0;
    memcpy(rgn->Get(_front), val, rgn->_unitSizeInByte);
}

void Deque::PushBack(void* val) {
    ++_back;
    ++_size;
    _front == _back ? --_front : 0;
    memcpy(rgn->Get(_back), val, rgn->_unitSizeInByte);
}

void Deque::PopFront() {
    ++_front;
    --_size;
}

void Deque::PopBack() {
    --_back;
    --_size;
}

addr Deque::GetFront() {
    return rgn->Get(_front);
}

addr Deque::GetBack() {
    return rgn->Get(_back);
}

addr Deque::Get(ui32 idx) {
    return rgn->Get(_front + idx);
}

//-------Stack-------

void Stack::SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx) {
    rgn = memReg;
    _back = begIdx;
    _base = _back;
    _cap = endIdx - begIdx + 1;
}

void Stack::SetUp(ui32 unitSize, ui32 cap) {
    rgn = &_ownedRgn;
    _ownedRgn.SetUp(unitSize, cap);
    _back  = 0;
    _base  = 0;
    _cap   = cap; 
}

void Stack::PushBack(void* val) {
    ++_back;
    memcpy(rgn->Get(_back), val, rgn->_unitSizeInByte);
}

void Stack::PopBack() {
    --_back;
}

addr Stack::GetBack() {
    return rgn->Get(_back);
}

addr Stack::Get(ui32 idx) {
    return rgn->Get(_base + idx);
}

//-------Doubly linked list-----------

DList::DList() {}
DList::DList(const DList& other) {
    _first = other._first;
    _last = other._last;
    _cap = other._cap;
    _base = other._base;
    _size = other._size;
    _ownedRgn = other._ownedRgn;
    rgn = other.rgn;
    if (other.rgn == &other._ownedRgn) {
        rgn = &_ownedRgn; 
    }
}

void DList::SetUp(ui32 unitSize, ui32 cap) {
    rgn = &_ownedRgn;
    _ownedRgn.SetUp(unitSize+8, cap, AllocStrat::DOUBLE);
    _cap    = cap; 
}

ui32 DList::GetPrevIdx(addr e) {
    return *CAST(ui32*,e); 
}

ui32 DList::GetNextIdx(addr e) {
    return *((ui32*)(CAST(byte*,e)+4+rgn->_unitSizeInByte-8));
}

void* DList::GetContent(addr ad) {
    return CAST(byte*,ad) + 4 ;
}

addr DList::_AddDetachedNode(const DlistNode* n, addr loc) {
    _AttachPrev(loc, n->_prev);
    _SetContent(loc, n->val);
    _AttachNext(loc, n->_next);
    return loc;
}

addr DList::_AddDetached(void* e) {
    DlistNode fn;
    addr loc;
    fn.val = e;
    fn._next = DLIST_POINT_TO_NONE;
    fn._prev = DLIST_POINT_TO_NONE;
    loc = rgn->Alloc();
    return _AddDetachedNode(&fn, loc);
}

ui64 DList::GetIdx(addr ad) {
    return rgn->_GetIndexFromAddr(ad);
}

void DList::_AttachPrev(addr ad, ui32 p) {
    *(CAST(ui32*, ad)) = p;
}

void DList::_AttachNext(addr ad, ui32 p) { 
    *((ui32*)(CAST(byte*, ad) + rgn->_unitSizeInByte - 4))= p;
}
    
void DList::_SetContent(addr ad, void* v) {
    memcpy(CAST(byte*,ad) + 4, v, rgn->_unitSizeInByte - 8);
}

addr DList::AddFirst(void* e) {
    if (_first == -1) {
        addr tmp = _AddDetached(e);
        _AttachPrev(tmp, DLIST_POINT_TO_NONE);
        _AttachNext(tmp, DLIST_POINT_TO_NONE);
        _first = _base;
        _last = _base;
        ++_size;
        return tmp;
    }
    addr n = _AddDetached(e);
    _AttachNext(n, _first);
    _AttachPrev(rgn->Get(_first + _base), GetIdx(n));
    _first = GetIdx(n);
    ++_size;
    return n;
}

addr DList::AddLast(void* element) {
    if (_size == 0) {
        return AddFirst(element);
    }
    addr n = _AddDetached(element);
    ui32 prev = GetIdx(rgn->Get(_last));
    _AttachNext(rgn->Get(_last), GetIdx(n));
    _AttachPrev(n, prev);
    _AttachNext(n, DLIST_POINT_TO_NONE);
    _last = GetIdx(n); 
    ++_size;
    return n;
}

addr DList::Erase(addr e) {
    ui32 idx = GetIdx(e);
    addr ret = GetNext(e);
    if (_size == 1) {
        rgn->Dealloc(e);
        --_size;
        return ret;
    }

    if (idx == _last) {
        addr p = GetPrev(e);
        _last = GetIdx(p);
        _AttachNext(p, DLIST_POINT_TO_NONE);
        rgn->Dealloc(e);
        --_size;
        return ret;
    }

    if (idx == _first) {
        addr n = GetNext(e);
        _first = GetIdx(n);
        _AttachPrev(n, DLIST_POINT_TO_NONE);
        rgn->Dealloc(e);
        --_size;
        return ret;
    }
    
    addr n = GetNext(e);
    addr p = GetPrev(e);
    _AttachNext(p, GetIdx(n));
    _AttachPrev(n, GetIdx(p));
    rgn->Dealloc(e);
    --_size;
    return ret;
}

addr DList::AddAfter(addr loc, void* e) {
    if (GetIdx(loc) == _last) {    
        return AddLast(e);
    }
    addr n = _AddDetached(e);
    _AttachPrev(GetNext(loc), GetIdx(loc));
    _AttachNext(n, GetIdx(GetNext(loc)));
    _AttachPrev(n, GetIdx(loc));
    ++_size;
    return n;
}

addr DList::GetNext(addr a) {
    ui64 idx = GetIdx(a);
    if (idx >= _size)
        return 0;
    return DirectGetNext(a);
}

addr DList::DirectGetNext(addr a) {
    ui32 v = READ(ui32, (CAST(byte*, a) + rgn->_unitSizeInByte - 4));
    return rgn->Get(v);
}

addr DList::GetPrev(addr a) {
    return rgn->Get(READ(ui32,a));
}

addr DList::GetLast() {
    return rgn->Get(_last);
}

addr DList::GetFirst() {
    return rgn->Get(_first);
}

addr DList::IterateAndGet(ui32 i) {
    addr p = GetFirst(); 
    for (int j = 0; j < i; ++j) {
        p = GetNext(p);
    }
    return p;
}


//---------------HashMaps------------------

static constexpr ui64 off = 14695981039346656037ULL;
static constexpr ui64 prim= 1099511628211ULL;

ui64 defaultHashFNV1A(const void* data, const ui32 sib) {
   ui64 hash = off;
   const ui8* bytes = CAST(ui8*, data);
   for (ui32 i = 0; i < sib; ++i) {
        hash ^= bytes[i];
        hash *= prim;
   }
   return hash;
}


#define MAP_UNIT_SIZE (_keySizeInBytes + _valueSizeInBytes )
void HashMap::SetUp(ui32 keySizeInBytes, ui32 valueSizeInBytes, ui32 collisionCap, ui32 cap) {
    _ownedRgn.SetUp(sizeof(DList), cap);
    rgn = &_ownedRgn;
    _collisionCap    = collisionCap;
    _keySizeInBytes  = keySizeInBytes;
    _valueSizeInBytes= valueSizeInBytes;
    for (int i = 0; i < cap; ++i) {
        *((ui32*)(CAST(byte*,_ownedRgn._data) + 4*i)) = 0;
    }
}



static bool CmpBytes(byte* b0, byte* b1, ui32 s) {
    for (ui32 i = 0; i < s; ++i) {
        if (b0[i] != b1[i])
            return 0;
    }
    return 1;
}

ui32 HashMap::_GetIdxFromKey(void* key) {
    ui64 h = hashFunc(key, _keySizeInBytes);    
    ui32 index = h % rgn->_size;  
    return index;
}

addr HashMap::Add(void* key, void* value) {
    ui32 index = _GetIdxFromKey(key);  
    HashData d = {key,value,index};
    return _Add(d);
}

addr HashMap::_Add(const HashData& d) {  
    addr ret = 0;
    ui32 idx = d.processedHash;
    ui32 n = _GetPtr(idx);
    addr adr = rgn->Get(idx);
    byte* buff = (byte*)malloc(MAP_UNIT_SIZE);
    memcpy(buff, d.key, _keySizeInBytes);
    memcpy(buff + _keySizeInBytes, d.data, _valueSizeInBytes);
    if (n == 0) {
        //Key not found
        DList* lp = &(*((DList*)(adr)) = DList());
        lp->SetUp(MAP_UNIT_SIZE, _collisionCap);
        ret = lp->AddFirst(buff);
        _IncPtr(idx);
        free(buff);
        return ret;
    }
    DList* lp = CAST(DList*, adr);
    byte* iterKey  = (byte*)malloc(_keySizeInBytes);
    addr iter = lp->GetFirst();
    while (iter) {
        memcpy(iterKey, lp->GetContent(iter), _keySizeInBytes);
        bool cmp = CmpBytes(iterKey, (byte*)d.key, _keySizeInBytes);
        if (cmp) {
            memcpy((byte*)lp->GetContent(iter)+_keySizeInBytes, d.data, _valueSizeInBytes);  
            free(buff);
            free(iterKey);
            return iter;
        }
        iter = lp->GetNext(iter);
    }
    ret = lp->AddLast(buff);
    _IncPtr(idx);
    free(buff);
    free(iterKey);
    return ret;
}

addr HashMap::Set(void* key, void* value) {
    ui32 index = _GetIdxFromKey(key);
    HashData d = {key,value,index};
    return _Set(d);
}

addr HashMap::_Set(const HashData& d) {
    addr ret = 0;
    ui32 idx = d.processedHash;
    ui32 n = _GetPtr(idx);
    addr adr = rgn->Get(idx);
    byte* buff = (byte*)malloc(MAP_UNIT_SIZE);
    memcpy(buff, d.key, _keySizeInBytes);
    memcpy(buff + _keySizeInBytes, d.data, _valueSizeInBytes);
    if (n == 0) {
        free(buff);
        return 0;
    }
    DList* lp = CAST(DList*, adr);
    addr iter = lp->GetFirst();
    byte* iterKey  = (byte*)malloc(_keySizeInBytes);

    while (iter) {
        memcpy(iterKey, lp->GetContent(iter), _keySizeInBytes);
        bool cmp = CmpBytes(iterKey, (byte*)d.key, _keySizeInBytes);
        //We found the key
        if (cmp) {
            memcpy((byte*)lp->GetContent(iter)+_keySizeInBytes, d.data, _valueSizeInBytes);  
            free(buff);
            free(iterKey);
            return iter;
        }
        //If we dont find we continue the loop
        iter = lp->GetNext(iter);
    }
    free(buff);
    free(iterKey);
    return 0;
}

bool HashMap::_Erase(const HashData& d) {
    ui32 idx = _GetIdxFromKey(d.key);
    ui32 n = _GetPtr(idx);
    if (n == 0)
        return 0; 
    addr adr = rgn->Get(idx);
    DList* lp = CAST(DList*, adr);
    byte* iterKey  = (byte*)malloc(_keySizeInBytes);
    addr iter = lp->GetFirst();
    while (iter) {
        memcpy(iterKey, lp->GetContent(iter), _keySizeInBytes);
        bool cmp = CmpBytes(iterKey, (byte*)d.key, _keySizeInBytes);
        //We found the key
        if (cmp) {
            lp->Erase(iter);
            _DecPtr(idx);
            free(iterKey);
            return 1;
        }
        //If we dont find we continue the loop
        iter = lp->GetNext(iter);
    }
    return 0;
}

bool HashMap::Erase(void* key) {
    ui32 index = _GetIdxFromKey(key);
    HashData d = {key,0,index};
    return _Erase(d);
}

addr HashMap::Get(void* key) {
    ui32 idx = _GetIdxFromKey(key);
    ui32 n = *(CAST(ui32*,rgn->_data)+ idx);
    if (n == 0)
        return 0; 
    addr adr = rgn->Get(idx);
    DList* lp = CAST(DList*, adr);
    byte* iterKey  = (byte*)malloc(_keySizeInBytes);
    addr iter = lp->GetFirst();
    while (iter) {
        memcpy(iterKey, lp->GetContent(iter), _keySizeInBytes);
        bool cmp = CmpBytes(iterKey, (byte*)key, _keySizeInBytes);
        if (cmp) {
            free(iterKey);
            return iter;
        }
        iter = lp->GetNext(iter);
    }
    return 0;
}

void* HashMap::ReadKey(addr ad) {
    return (void*)(CAST(byte*,ad) + sizeof(int));
}

void* HashMap::ReadValue(addr ad) {
    return (void*)(CAST(byte*,ad) + _keySizeInBytes + sizeof(int));//WARNING:: list padding for index is Hardcoded here
}

void* HashMap::GetValue(void* key) {
    addr ad = Get(key); 
    return !ad ? 0 : ReadValue(ad); 
}

ui32 HashMap::_IncPtr(ui32 idx) {
    return *(CAST(ui32*,rgn->_data)+ idx) = _GetPtr(idx) + 1;
}

ui32 HashMap::_DecPtr(ui32 idx) {
    return *(CAST(ui32*,rgn->_data)+ idx) = _GetPtr(idx) - 1;
}

ui32 HashMap::_GetPtr(ui32 idx) {
    return *(CAST(ui32*,rgn->_data)+ idx);
}


void String::SetUp(MemoryRegion* memReg, ui32 begIdx, ui32 endIdx) {
    return container.SetUp(memReg, begIdx, endIdx);
}

void String::SetUp(ui32 cap) {
    return container.SetUp(sizeof(char), cap);
}

char String::GetChar(int i) {
    return *(char*)container.Get(i);
}

void String::Insert(char c) {
    container.PushBack(&c);
}

void String::Concat(String& other) {
    for (int i = 0; i < other.container._size; ++i) {
        Insert(other.GetChar(i));
    }
    char endc = '\0';
    container.PushBack(&endc);
}

void String::Concat(const char* other) {
    char c;
    int i = 0;
    while (c = other[i++]) {
        Insert(c);
    }
    char endc = '\0';
    container.PushBack(&endc);
}

const char* String::GetData() {
    return (const char*)container.Get(0);
}
