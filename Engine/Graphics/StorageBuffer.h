#pragma once
#include "Globals.h"

enum StorageBufferRO {
    NW_SSBO_READ = 0x88B8,
    NW_SSBO_WRT  = 0x88B9,
    NW_SSBO_RW = 0x88BA,
};

class StorageBuffer {
    public:
        uint32 _glID = 0;
        uint32 _size;
        void*  ptr = 0;
        void SetUp(ui32 size, void* data = 0, bool isStatic = 0);  
        void SyncForCPU();
        void Bind(ui32 slot = 0);
        void BindBuff();
        void Wrt(void* data, ui32 size, ui32 offset = 0);
        void DirectWrt(void* data, ui32 size, ui32 offset = 0);
        void Read(void* data, ui32 size, ui32 offset = 0);
        void DirectRead(void* data, ui32 size, ui32 offset = 0);
        void** Map(StorageBufferRO = NW_SSBO_RW);
        void Unmap();
        void Delete();

        inline uint32 GetSizeInByte() {return _size;}
};


