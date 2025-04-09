#pragma once
#include <vector>
#include <deque>

template<typename T>

#define NW_Pool_Func_Type(f) void(*f)(T*,void*)

class NWPool {
    public:
        std::vector<T>   _container;
        std::deque<int> _available;
        std::deque<int> _active;
        bool _recycleFlag = 0;
        
        void SetRecycle(bool r = 1) {
            _recycleFlag = r;
        }

        bool GetRecycle() {
            return _recycleFlag;
        }

        void SetUp(int s) {
            _active.clear();
            _container.resize(s);
            for (int i = 0; i < s; ++i) _available.push_back(i);
        }

        void MapTo(NW_Pool_Func_Type(f), void* d, int index) {
            for (int i = 0; i < index; ++i) {
                f(_container[_active[i]], d);
            }
        }

        void MapAll(NW_Pool_Func_Type(f), void* d) {
            MapTo(f,d,_container.size());
        }

        void Map(NW_Pool_Func_Type(f), void* d) {
            MapTo(f,d,_active.size());
        }

        int GetNextIdx() {
            if (_available.size() != 0) {
                int c = _available.front();
                _available.pop_front();
                _active.push_back(c);
                return c;
            }
            if (!_recycleFlag) {
                return -1;
            }
            int c = _active[0];
            _active.pop_front();
            _available.push_back(c);    
            return GetNextIdx(); 
        }

        T* GetNext() {
            int n = GetNextIdx();
            if (n == -1) return 0;
            return _container[n];
        };

        T* GetNextAndApply(NW_Pool_Func_Type(f), void* d = 0) {
            T* e = GetNext();
            if (!e) return e;
            f(e,d);
            return e; 
        }

        int PeekNextIdx() const {
            if (_available.size() != 0) {
                return _available.front();
            }
            if (!_recycleFlag) {
                return -1;
            }
            return _active[0];
        }

        const T* PeekNext() const {
            int n = PeekNextIdx();
            if (n == -1) return 0;
            return _container[n];
        };
};
