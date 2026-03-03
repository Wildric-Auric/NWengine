#include "StorageBuffer.h"
#include "Context.h"
#include "GL/glew.h"

void StorageBuffer::SetUp(uint32 size, void* data, bool isStatic) {
	_size = size;
	NW_GL_CALL(glGenBuffers(1, &_glID));
	BindBuff();
	NW_GL_CALL(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, isStatic ? GL_STATIC_COPY : GL_DYNAMIC_COPY));
}

void StorageBuffer::Bind(ui32 slot) { NW_GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, _glID)); }

void StorageBuffer::SyncForCPU() {
	BindBuff();
	Context::NWMemoryBarrier(NWMemoryBarrierBit::BUFFER_UPDATE_BARRIER_BIT |
							 NWMemoryBarrierBit::CLIENT_MAPPED_BUFFER_BARRIER_BIT);
}

void StorageBuffer::BindBuff() { NW_GL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, _glID)); }

void** StorageBuffer::Map(StorageBufferRO access) {
	BindBuff();
	NW_GL_CALL((ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, access)));
	return &ptr;
}

void StorageBuffer::Wrt(void* data, ui32 size, ui32 offset) {
	Unmap();
	SyncForCPU();
	Map(NW_SSBO_WRT);
	DirectWrt(data, size, offset);
	Unmap();
}

void StorageBuffer::DirectWrt(void* data, ui32 s, ui32 offset) { NWmemcpy((char*)ptr + offset, data, s); }

void StorageBuffer::Read(void* data, ui32 size, ui32 offset) {
	Unmap();
	SyncForCPU();
	Map(NW_SSBO_READ);
	DirectRead(data, size, offset);
	Unmap();
}

void StorageBuffer::DirectRead(void* data, ui32 size, ui32 offset) { NWmemcpy((char*)data, (char*)ptr + offset, size); }

void StorageBuffer::Unmap() {
	if(!ptr)
		return;
	NW_GL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, _glID));
	NW_GL_CALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
	ptr = 0;
}

void StorageBuffer::Delete() {
	Unmap();
	NW_GL_CALL(glDeleteBuffers(1, &_glID));
    _glID = 0;
}
