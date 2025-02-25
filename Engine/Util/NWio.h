#pragma once

enum class StreamCursorPos {
    CURSOR_BEG,
    CURSOR_END,
    CURSOR_CURRENT
};

class InputStream {
    public:
    void* _str;
    bool Open(const char* path, bool isBinary = 0);
    bool HasFinished();
    void Close();
    void SetStrCursor(int i, StreamCursorPos relative);
    void Read(void* output, int n);
    void ReadDWORD(int* output);
    void ReadByte(char* output);
};
