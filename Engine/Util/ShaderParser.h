#pragma once
#include <list>
#include <string>
#include <unordered_map>

typedef void(*ShaderParserStateFunc)(void*);
typedef void(*ShaderParserTokStateFunc)(void*);
typedef bool(*ShaderParserGetNxtFunc)(void*);

enum class ShaderType {
    NONE,
    VERT,
    FRAG
};

struct ShaderParserUniformData {
    std::string name = "";
    std::string type = ""; 
    std::string defaultValue = ""; 
    int location = -1;
};

class ShaderParser {
    public:
    char c;
    char lc;
    std::list<std::string> tokens;
    std::list<std::string>::iterator tokenIter;
    std::string* ltoken;
    std::string  curToken;
    std::string* curShaderTxt;
    ShaderType curType;
    const char* cPtr;

    bool macroNxt;
    bool noWhiteSpace;
    bool dontInc;
    int  scope;
    int  scope2;

    void* file;

    ShaderParserStateFunc func;
    ShaderParserTokStateFunc tokenFunc;
    ShaderParserGetNxtFunc   getNext; 

    std::unordered_map<std::string, std::string> constants;
    std::unordered_map<std::string, ShaderParserUniformData> uniformsData;
    std::string vert = "";
    std::string frag = "";
    std::string shaderVersion = "";
    int fragAttNum;

    ShaderParser();
    bool PushToken(); 
    void PopToken();
    void Tokenize();
    void ProcessTokens();
    void _Parse();
    void Parse(const char* src);
    void ParseFromPath(const char* path);
    bool GetNext();
    void Reset();
    void AddCnst(const char* id, const char* value);
    void ClearCnsts();
    void OutputData();

    inline const std::string& GetVertTxt() const { return vert;}
    inline const std::string& GetFragTxt() const { return frag;}
    inline const int GetAttNum() const { return fragAttNum;}
    inline const std::unordered_map<std::string, ShaderParserUniformData>& GetUniforms() const { return uniformsData;}
    inline const std::string& GetShaderVersion() const { return shaderVersion;}
};


