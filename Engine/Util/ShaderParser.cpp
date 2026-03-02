#include "ShaderParser.h"
#include <fstream>
#include <iostream>
#include <vector>

static bool isCommentBeg(const std::string& s) { return s == "/*"; }
static bool isCommentEnd(const std::string& s) { return s == "*/"; }
static bool isSpace(char c) { return c == ' ' || c == '\t'; }

void General(void* ptr);
void Comment0(void* ptr);
void Comment1(void* ptr);
void Preprocessor(void* ptr);

void TokenGeneral(void* ptr);
void TokenDirective(void* ptr);

void Comment0(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	if(p.c == '\n') {
		p.func	   = General;
		p.curToken = "";
		return;
	}
}

void Comment1(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	if(p.lc != '*' || p.c != '/')
		return;
	p.func	   = General;
	p.curToken = "";
}

void Preprocessor(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	if(p.c == '\\') {
		p.macroNxt = 1;
		return;
	} else if(isSpace(p.c) && !isSpace(p.curToken.back())) {
		p.curToken += ' ';
		return;
	} else if((p.c == ' ' || p.c == '\t')) {
		return;
	} else if(p.c == '\n' && p.macroNxt) {
		p.macroNxt = 0;
		return;
	} else if(p.c != '\n') {
		p.curToken += p.c;
		return;
	}
	p.func = General;
	p.PushToken();
}

bool IsOp(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '&' || c == '|' || c == '!' || c == '>' || c == '<' ||
		   c == '=';
}

bool IsSep(char c) {
    return c == ',' || c == '{' || c == '}' || 
           c == '(' || c == ')' || c == '[' || 
           c == ']' || c == ';';
}

bool IsValidTokenChar(char c) { return c != '\n' && c != ' ' && c != '\t'; }
bool IsTokenEnd(char c) {       return c == '\t' || c == ' ' || c == '\n'; }
bool IsNum(char c) {            return c >= '0' && c <= '9'; }

void ExpNum(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	if(!IsNum(p.c)) {
		p.PushToken();
		p.func = General;
		if(IsValidTokenChar(p.c))
			p.curToken += p.c;
		return;
	}
	p.curToken += p.c;
}

bool IsTokenNumEBegin(const std::string& tok) {
    //a number must preceed e; hence < 2 not != 0
	if(tok.size() < 2)
		return 0;
	if(tok[tok.size() - 1] != 'e')
		return 0;
	for(int i = 0; i < tok.size() - 1; ++i) {
		if(!IsNum(tok[i]))
			return 0;
	}
	return 1;
}

void General(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
    if (p.preProcDir) {
        if (p.c == '\\') {
            p.macroNxt = 1;
            return;
        }
        if (p.c == '\n' && !p.macroNxt) {
            p.PushToken();
            p.tokens.push_back("#$");
            p.preProcDir = 0;
            return;
        }
        if (p.c == '\n' && p.macroNxt) {
            p.macroNxt = 0;
            return;
        }
    }
    if(IsTokenEnd(p.c)) {
		p.PushToken();
	} else if(p.c == '/' && p.lc == '/') {
		p.func = Comment0;
		p.PopToken();
		p.curToken = "";
	} else if(p.lc == '/' && p.c == '*') {
		p.func	   = Comment1;
        p.PopToken();
		p.curToken = "";
	} else if(IsTokenNumEBegin(p.curToken) && (IsNum(p.c) || p.c == '+' || p.c == '-')) {
		p.func = ExpNum;
	} else if(IsSep(p.c) || IsOp(p.c)) {
		p.PushToken();
		p.curToken = p.c;
		p.PushToken();
		return;
	} else if(p.c == '#') {
		p.curToken += p.c;
        p.PushToken();
        p.preProcDir = 1;
		//p.func  = Preprocessor;
		return;
	}

	if(IsValidTokenChar(p.c))
		p.curToken += p.c;
}

bool ShaderParser::PushToken() {
	if(curToken == "")
		return 0;
	tokens.push_back(curToken);
	curToken = "";
	return 1;
}

void ShaderParser::PopToken() {
	if(!tokens.empty())
		tokens.pop_back();
}

void ShaderParser::Reset() {
	curShaderTxt  = &unknw;
	cPtr		  = 0;
	c			  = 0;
	lc			  = 0;
	curToken	  = "";
	ltoken		  = 0;
	macroNxt	  = 0;
	dontInc		  = 0;
	scope		  = 0;
	scope2		  = 0;
	file		  = 0;
	curType		  = ShaderType::NONE;
    vert          = "";
    frag          = "";
    comp          = "";
    unknw         = "";
    preProcDir    =  0;
	shaderVersion = "";
    prgmaInfo.clear();  
    locs.clear();
	tokens.clear();
	uniformsData.clear();
	enabledAtts.clear();
}

ShaderParser::ShaderParser() {
	this->func		= &General;
	this->tokenFunc = &TokenGeneral;
}

void ShaderParser::Tokenize() {
	std::string curToken = "";
	while(GetNext()) {
		func(this);
	}
}

bool GetNextFromPath(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	std::fstream& f = *(std::fstream*)(p.file);
	p.lc			= p.c;
	p.c				= f.get();
	return p.c != EOF;
}

bool GetNextFromRaw(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	p.lc			= p.c;
	return (p.cPtr++ && (p.c = *p.cPtr));
}

bool ShaderParser::GetNext() { return getNext(this); }

void SplitStr(const std::string& str, std::vector<std::string>& container) {
	container.push_back("");
	for(int i = 0; i < str.size(); ++i) {
		if(str[i] == ' ') {
			container.push_back("");
			continue;
		}
		container.back() += str[i];
	}
}

void TokenDirective(void* ptr) {
	ShaderParser&			 p = *(ShaderParser*)ptr;
	std::vector<std::string*> container;
	//SplitStr(*p.tokenIter, container); 
	//bool prgma = container.size() > 0 && container[0] == "#pragma";
    //todo::add get next token and extra checks
    int i = 0;
    while (p.tokenIter != p.tokens.end() && *p.tokenIter != "#$") {
        container.push_back(&*p.tokenIter);
        p.tokenIter++;
        ++i; 
    } 
    for (int j = 0; j < i; ++j)
        p.tokenIter--;
    bool prgma  = container.size() && (*container[0] == "pragma");
    if (prgma) 
        p.prgmaInfo.push_back({p.tokenIndex, (uint16_t)i});
    std::string* hint = container.size() <= 1 ? 0 : container[1];
	if(prgma && (*hint == "vertex" || *hint == "VertexShader")) {
		//p.tokenIter  = p.tokens.erase(p.tokenIter);
		p.curType	   = ShaderType::VERT;
		p.dontInc	   = 1;
		p.tokenFunc	   = TokenGeneral;
		p.curShaderTxt = &p.vert;
        p.locs.push_back({p.curType, &p.vert, (uint32_t)p.tokenIndex-1});
	}
	else if(prgma && (*hint == "fragment" || *hint == "FragmentShader")) {
		//p.tokenIter  = p.tokens.erase(p.tokenIter); deprecated, should erase the whole directive
		p.curType	   = ShaderType::FRAG;
		p.dontInc	   = 1;
		p.tokenFunc	   = TokenGeneral;
		p.curShaderTxt = &p.frag;
        p.locs.push_back({p.curType, &p.frag, (uint32_t)p.tokenIndex-1});
	} 
	else if(prgma && (*hint == "compute" || *hint == "ComputeShader")) {
		//p.tokenIter	   = p.tokens.erase(p.tokenIter);
		p.curType	   = ShaderType::COMPUTE;
		p.dontInc	   = 1;
		p.tokenFunc	   = TokenGeneral;
		p.curShaderTxt = &p.comp;
        p.locs.push_back({p.curType, &p.comp, (uint32_t)p.tokenIndex-1});
	}
	else if( prgma && container.size() >= 3 && *container[1] == "def") {
		p.tokenIter++;
		auto pp = p.constants.find(*container[3]);
		if(pp == p.constants.end()) {
			p.tokens.insert(p.tokenIter, std::string("#define ") + *container[2] + " " + *container[3]);
		} else {
			p.tokens.insert(p.tokenIter, std::string("#define ") + *container[2] + " " + pp->second);
		}
		p.tokenIter--;
		p.tokenIter--;
		//p.tokenIter = p.tokens.erase(p.tokenIter);
		p.dontInc	= 1;
		p.tokenFunc = TokenGeneral;
	} else if(container.size() >= 2 && *container[0] == "version") {
		p.shaderVersion = *container[1];
		p.tokenFunc = TokenGeneral;
	}
	else {
		 p.tokenFunc = TokenGeneral;
	}
}

void TokenUniform(void* ptr) {
	ShaderParser&					 p	   = *(ShaderParser*)ptr;
	ShaderParserUniformData			 uni   = {};
	std::list<std::string>::iterator other = p.tokenIter;
	// Get location if it exists:
	other--;
	if(*p.tokenIter == ")") {
		p.tokenIter--;
        p.tokenIndex--;
		std::string& tmp = *p.tokenIter;
		uni.location	 = std::stoi(tmp);
	}
	other = p.tokenIter;
	// Get other data
	uni.type = *(++other);
	uni.name = *(++other);
	other++;

	// Get size if its an array
	while(*other != ";" && *other != "=") {
		uni.type += *other;
		other++;
	}

	if(*(other++) == "=") {
		uni.defaultValue = "";
		while(*other != ";" && other != p.tokens.end()) {
			uni.defaultValue += *other;
			++other;
		}
		other++;
	}
	p.uniformsData[uni.name] = uni;
	p.tokenIter				 = other;
	p.dontInc				 = 1;
}

void TokenGeneral(void* ptr) {
	ShaderParser& p = *(ShaderParser*)ptr;
	if(p.tokenIter->size() > 0 && *p.tokenIter == "#") {
		p.tokenFunc = TokenDirective;
        //++p.tokenIter;
		//TokenDirective(ptr); 
	} else if(*p.tokenIter == "uniform") {
		//TokenUniform(ptr); NO REFLECTED UNIFORMS ANYMORE TODO::CHECK CORRECTNESS
	} else if(*p.tokenIter == "out" && p.curType == ShaderType::FRAG && p.scope == 0 && p.scope2 == 0) {
		uint16_t						 loc	 = p.enabledAtts.empty() ? 0 : p.enabledAtts.back() + 1;
		std::list<std::string>::iterator otherIt = p.tokenIter;
		if(*(--otherIt) == ")") {
			--otherIt;
			loc = stoi(*otherIt);
		}
		p.enabledAtts.push_back(loc);
	}
	p.scope += (*p.tokenIter == "{") - (*p.tokenIter == "}");
	p.scope2 += (*p.tokenIter == "(") - (*p.tokenIter == ")");
}

void ShaderParser::ProcessTokens() {
	tokenIter  = tokens.begin();
    tokenIndex = 0;
	while(tokenIter != tokens.end()) {
		tokenFunc(this);
		if(dontInc) {
			dontInc = 0;
			continue;
		}
		ltoken = &*tokenIter;
		tokenIter++;
        tokenIndex++;
	}
}

void ShaderParser::FillShaderText() {
    if (curType == ShaderType::NONE || locs[0].loc) {
        printf("Shader must start with: pragma <type>, where type is either vertex, fragment or compute.");
        return;
    }
    bool skipPrg = 0;
    uint32_t index = 0;
    int cur   = 0;
    for (auto tokenIter = tokens.begin(); tokenIter != tokens.end(); tokenIter++) {
        if (cur < locs.size() && index == locs[cur].loc) {
            curShaderTxt = locs[cur].txt;
            curType      = locs[cur].type;
            ++cur;
        }
        if (*tokenIter  ==  "#$") {
			if(!skipPrg) {
				*curShaderTxt += '\n';
			}
            skipPrg = 0;
        }
        else if (skipPrg) {
            void();
        }
        else if (*tokenIter == "pragma" && curShaderTxt->back() == '#') {
            curShaderTxt->pop_back();
            skipPrg = 1; 
        }
        else if (*tokenIter == "#") {
            *curShaderTxt += "#";
        }
        else if (*tokenIter == ";" || *tokenIter == "}" || *tokenIter == "{") {
            if (curShaderTxt->size() && isSpace(curShaderTxt->back()))
			    curShaderTxt->pop_back();
            *curShaderTxt += *tokenIter + "\n";
        } 
        else if(IsOp((*tokenIter)[0])) {
            *curShaderTxt += *tokenIter;
        } 
        else if (IsSep((*tokenIter)[0])) {
            if (curShaderTxt->size() && isSpace(curShaderTxt->back()))
			    curShaderTxt->pop_back();
			*curShaderTxt += *tokenIter;
        }
        else {
			*curShaderTxt += *tokenIter + " ";
        }

        index++;
    }
}

void ShaderParser::AddCnst(const char* id, const char* value) { constants[id] = value; }

void ShaderParser::ClearCnsts() { constants.clear(); }


void ShaderParser::_Parse() {
	Tokenize();
	ProcessTokens();
    FillShaderText();
}

void ShaderParser::Parse(const char* src) {
	if(src == 0)
		return;
	Reset();
	cPtr	= src;
	getNext = GetNextFromRaw;
	_Parse();
}

void ShaderParser::ParseFromPath(const char* path) {
	if(path == 0)
		return;
	Reset();
	std::fstream fs;
	fs.open(path);
	if(!fs.is_open()) {
		std::cout << "Warning: File not found -> " << path << std::endl; // TODO::
		fs.close();
		return;
	}
	getNext = GetNextFromPath;
	file	= &fs;
	_Parse();
	fs.close();
}

void ShaderParser::OutputPrgmaInfo() {
    for (auto iter = prgmaInfo.begin(); iter != prgmaInfo.end(); iter++) {
        printf("(%d, %d)", iter->pos, iter->len);
    }
}

void ShaderParser::OutputTokens(int n) {
    int i =0;
    for (std::string& tok : tokens) {
        if (tok[0] != '\n')
            printf("[%s]", tok.c_str()); 
        else 
            printf("[Endline]");
        if (i == n) {
            printf("\n");
            i = 0;
        }
        else printf(" ");
        ++i;
    }
    printf("\n");
}

void ShaderParser::OutputData() {
	if(comp == "") {
		std::cout << "--------Vertex--------" << std::endl;
		std::cout << vert << std::endl;
		std::cout << "--------Fragment--------" << std::endl;
		std::cout << frag << std::endl;
	} else {
		std::cout << "--------Compute--------" << std::endl;
		std::cout << comp << std::endl;
	}

	std::cout << "-------\nAtts Num: " << GetEnabledAtts().size() << std::endl;
	std::cout << "-------Used Atts---------- " << std::endl;
	for(auto& i : GetEnabledAtts()) {
		std::cout << i << std::endl;
	}
	std::cout << "-------\nShader Vers: " << GetShaderVersion() << std::endl;

	std::cout << "--------Uniforms--------" << std::endl;
	for(auto& uni : uniformsData) {
		std::cout << uni.second.name << ": " << uni.second.type << ((uni.second.defaultValue.size() > 0) ? " = " : "")
				  << uni.second.defaultValue;
		std::cout << "\n";
	}
}
