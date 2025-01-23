#include "ShaderParser.h"
#include <vector>
#include <iostream>
#include <fstream>

static bool isCommentBeg(const std::string& s) {return s == "/*";}
static bool isCommentEnd(const std::string& s) {return s == "*/";}


void General(void* ptr); 
void Comment0(void* ptr); 
void Comment1(void* ptr); 
void Preprocessor(void* ptr); 

void TokenGeneral(void* ptr);
void TokenDirective(void* ptr);


void Comment0(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;
    if (p.c == '\n') {
        p.func = General;
        p.curToken = "";
        return;
    }
}

void Comment1(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;
    if (p.c != '/' || p.lc != '*') return;
    p.func = General;
    p.curToken = "";
}

void Preprocessor(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;
    if (p.c == '\\') {
        p.macroNxt = 1;
        return;
        }
    else if ((p.c == ' ' || p.c == '\t') && p.curToken.back() != ' ') {
        p.curToken += ' ';
        return;
    }
    else if ((p.c == ' ' || p.c == '\t')) {
        return;
    }
    else if (p.c == '\n' && p.macroNxt) {
        p.macroNxt = 0;
        return;
    }
    else if (p.c != '\n') {
        p.curToken += p.c; 
        return;
    }
    p.func = General;
    p.PushToken();
}

bool IsOp(char c) {
   return c == '+' || c == '-' || c == '*' || c == '/'
       || c == '^' || c == '&' || c == '|' || c == '!'
       || c == '>' || c == '<' || c == '=';
}

bool IsValidTokenChar(char c) 
{ return c != '\n' && c != ' ' && c != '\t';}
bool IsTokenEnd(char c) 
{ return c == ' ' || c == '\n'; }

void General(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;

    if (IsTokenEnd(p.c)) {
        p.PushToken();
    }
    else if (p.c == '/' && p.lc == '/') {
        p.func = Comment0;  
        p.PopToken();
        p.curToken = "";
    }
    else if (p.c == '/' && p.lc == '*') {
        p.func = Comment1;    
        p.curToken = "";
    } 
    else if (p.c == '{' || p.c == '}' || p.c == '(' || p.c == ')' 
          || p.c == '[' || p.c == ']' || p.c == ';' || IsOp(p.c)) 
    {
        p.PushToken();
        p.curToken = p.c; 
        p.PushToken();
        return;
    }
    else if (p.c == '#') {
        p.curToken += p.c;
        p.func = Preprocessor;
        return;
    } 


    if (IsValidTokenChar(p.c))
        p.curToken += p.c;
}

bool ShaderParser::PushToken() { 
        if (curToken == "") return 0;
        tokens.push_back(curToken);
        curToken = "";
        return 1;
}

void ShaderParser::PopToken() {
    if (!tokens.empty()) tokens.pop_back();
}

void ShaderParser::Reset() {
    noWhiteSpace = 0;
    curShaderTxt = 0;
    cPtr      = 0; 
    c         = 0;
    lc        = 0;
    curToken  = "";
    ltoken    = 0;
    macroNxt = 0; 
    dontInc  = 0;
    scope     = 0;
    scope2    = 0;
    file     = 0;
    curType = ShaderType::NONE;
    vert = "";
    frag = "";
    shaderVersion = "";
    tokens.clear();
    uniformsData.clear();
    fragAttNum  = 0;
}

ShaderParser::ShaderParser() {
    this->func = &General;
    this->tokenFunc = &TokenGeneral;
}

void ShaderParser::Tokenize() {
    std::string curToken = "";
    while (GetNext()) {
        func(this);
    }
}

bool GetNextFromPath(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;
    std::fstream& f = *(std::fstream*)(p.file); 
    p.lc = p.c;
    p.c  = f.get();
    return (p.c > 0);
}

bool GetNextFromRaw(void* ptr){
    ShaderParser& p = *(ShaderParser*)ptr;
    p.lc = p.c;
    return (p.cPtr++ && (p.c = *p.cPtr));
}

bool ShaderParser::GetNext() {
     return getNext(this);
}

void SplitStr(const std::string& str, std::vector<std::string>& container) {
    container.push_back("");
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ') {
            container.push_back("");
            continue;
        }
        container.back() += str[i]; 
    }
}

void TokenDirective(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;
    std::vector<std::string> container;
    SplitStr(*p.tokenIter, container);

    bool prgma = container.size() > 0 && container[0] == "#pragma";

    if (container.size() >= 2 && prgma && container[1] == "vertex") {
        p.tokenIter = p.tokens.erase(p.tokenIter);
        p.curType = ShaderType::VERT;
        p.dontInc = 1;
        p.tokenFunc = TokenGeneral;
        p.curShaderTxt = &p.vert;
    }

    else if (container.size() >= 2 && prgma && container[1] == "fragment") {
        p.tokenIter = p.tokens.erase(p.tokenIter);
        p.curType = ShaderType::FRAG;
        p.dontInc = 1;
        p.tokenFunc = TokenGeneral;
        p.curShaderTxt = &p.frag;
    }

    else if (container.size() >= 3 && prgma && container[1] == "def") 
    {
        p.tokenIter++;
        auto pp = p.constants.find(container[3]);
        if (pp == p.constants.end()) {
            p.tokens.insert(p.tokenIter, std::string("#define ") 
                    + container[2] + " " + container[3]);
        }
        else {
            p.tokens.insert(p.tokenIter, std::string("#define ") 
                    + container[2] + " " + pp->second);
        }


        p.tokenIter--; p.tokenIter--;
        p.tokenIter =p.tokens.erase(p.tokenIter);
        p.dontInc = 1;
        p.tokenFunc = TokenGeneral;
    }
    else if (container.size() >= 2 && container[0] == "#version") {
        p.shaderVersion = container[1];
        *p.tokenIter   += "\n";
         p.tokenFunc    = TokenGeneral;
    }
    
    else {
        *p.tokenIter += "\n";
         p.tokenFunc = TokenGeneral;
    }
    p.noWhiteSpace = 1;
}

void TokenUniform(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;
    ShaderParserUniformData uni = {};
    std::list<std::string>::iterator other = p.tokenIter;
    //Get location if it exists:
    other--;
    if (*p.tokenIter == ")") {
        p.tokenIter--;
        std::string& tmp = *p.tokenIter;
        uni.location = std::stoi(tmp);        
    }
    other = p.tokenIter;
    //Get other data
    *p.curShaderTxt += "uniform ";
    uni.type = *(++other);
    *p.curShaderTxt += uni.type + " ";
    uni.name = *(++other);
    *p.curShaderTxt += uni.name + " ";
    other++;

    //Get size if its an array
    while (*other != ";" && *other != "=") {
        uni.type += *other;
        *p.curShaderTxt += *other;
        other++;
    }

    if (*(other++) == "=") {
        *p.curShaderTxt += "= ";
        uni.defaultValue = "";
        while (*other != ";" && other != p.tokens.end())  {
            *p.curShaderTxt  += *other + " ";
            uni.defaultValue += *other;  
            ++other;
        }
        other++;
    }

    *p.curShaderTxt += ";\n"; 
    p.uniformsData[uni.name] = uni;
    p.tokenIter = other;
    p.dontInc = 1;
}

void TokenGeneral(void* ptr) {
    ShaderParser& p = *(ShaderParser*)ptr;

    if (p.tokenIter->size() > 0 && (*p.tokenIter)[0] == '#') {
       p.tokenFunc = TokenDirective; 
       TokenDirective(ptr);
    }
    else if (*p.tokenIter == "uniform") {
        TokenUniform(ptr);
    }
    else if (*p.tokenIter == "out" && p.curType == ShaderType::FRAG && p.scope == 0 && p.scope2 == 0) {
        p.fragAttNum += 1;
    }

    p.scope  += (*p.tokenIter == "{") - (*p.tokenIter == "}");     
    p.scope2 += (*p.tokenIter == "(") - (*p.tokenIter == ")");
}

void ShaderParser::ProcessTokens() {
    tokenIter = tokens.begin();
    while (tokenIter != tokens.end()) {
        tokenFunc(this);
        if (dontInc) {
            dontInc = 0;
            continue;
        }

        if (&*tokenIter != ltoken && curType != ShaderType::NONE) { 
                if (*tokenIter == ";") {
                    *curShaderTxt += *tokenIter + "\n";
                }
                else if (IsOp((*tokenIter)[0]) || noWhiteSpace) {
                    *curShaderTxt += *tokenIter;
                    noWhiteSpace = 0;
                }
                else
                    *curShaderTxt += *tokenIter + ' ';
        }
        ltoken = &*tokenIter; 
        tokenIter++;
        
    }
}

void ShaderParser::AddCnst(const char* id, const char* value) {
    constants[id] = value;
}

void ShaderParser::ClearCnsts() {
    constants.clear();
}

void ShaderParser::_Parse() {
    Tokenize();
    ProcessTokens();
}

void ShaderParser::Parse(const char* src) {
    if (src == 0) return;
    Reset();
    cPtr = src;
    getNext = GetNextFromRaw;
    _Parse();
}
    
void ShaderParser::ParseFromPath(const char* path) {
    if (path == 0) return;
    Reset();
    std::fstream fs;
    fs.open(path);
    if (!fs.is_open()) {
        std::cout << "Warning: File not found -> " << path << std::endl; //TODO::
        fs.close();
        return;
    }
    getNext = GetNextFromPath;  
    file = &fs;
    _Parse();
    fs.close();
}

void ShaderParser::OutputData() {
    std::cout << "--------Vertex--------"<< std::endl;
    std::cout << vert <<std::endl;
    std::cout << "--------Fragment--------"<< std::endl;
    std::cout << frag<<std::endl;

    std::cout << "-------\nAtts Num: " << GetAttNum() << std::endl;
    std::cout << "-------\nShader Vers: " << GetShaderVersion() << std::endl;

    std::cout << "--------Uniforms--------"<< std::endl;
    for (auto& uni : uniformsData) {
        std::cout << uni.second.name << ": " << uni.second.type 
                  << ((uni.second.defaultValue.size() > 0) ? " = " : "")
                  << uni.second.defaultValue;
        std::cout << "\n";
    }
}
