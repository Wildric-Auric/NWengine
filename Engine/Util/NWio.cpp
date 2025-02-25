#include "NWio.h"
#include <fstream>


bool InputStream::Open(const char* path, bool isBinary) {
    std::fstream& str = *new std::fstream();
    _str = &str;
    str.open(path, (isBinary ? std::ios::binary : 0) | std::ios::in);
    return str.is_open();
}

bool InputStream::HasFinished() {
   return ((std::fstream*)_str)->eof();
}

void InputStream::Close() {
    ((std::fstream*)_str)->close();
    delete ((std::fstream*)_str);
}

void InputStream::SetStrCursor(int i, StreamCursorPos relative) {
    std::fstream* s = ((std::fstream*)_str);
    s->seekg(i, relative == StreamCursorPos::CURSOR_BEG ? std::ios::beg : (relative == StreamCursorPos::CURSOR_END ? std::ios::end : std::ios::cur));
}

void InputStream::Read(void* output, int n) {
    std::fstream* s = ((std::fstream*)_str);
    s->read((char*)output, n);
}

void InputStream::ReadDWORD(int* output) {
    std::fstream* s = ((std::fstream*)_str);
    s->read((char*)output, 4);
    *output = *output; 
}

void InputStream::ReadByte(char* output) {
    std::fstream* s = ((std::fstream*)_str);
    s->read((char*)output, 1);
}
