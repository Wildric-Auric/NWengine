#include "Shader.h"
#include "FrameBuffer.h"
#include "Globals.h"
#include <GL/glew.h>
#include <fstream>
#include <string.h>

ShaderParser Shader::parser;

bool CheckShaderCompileError(int shader, const char* code, const char* txt) {
	char log[512];
	int	 successInfo;
	NW_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &successInfo));
	if(!successInfo) {
		NW_GL_CALL(glGetShaderInfoLog(shader, 512, NULL, log));
		NW_LOG_ERROR((std::string(txt) + code).c_str());
		NW_LOG_ERROR(log);
	}
	return successInfo;
}

bool CheckShaderLinkError(int prg) {
	char log[512];
	int	 successInfo;
	NW_GL_CALL(glGetProgramiv(prg, GL_LINK_STATUS, &successInfo));
	if(!successInfo) {
		NW_GL_CALL(glGetProgramInfoLog(prg, 512, NULL, log));
		NW_LOG_ERROR((std::string("SHADER::LINKAGE FAILED")).c_str());
		NW_LOG_ERROR(log);
	}
	return successInfo;
}

ShaderText Shader::fastParseShader(const char* path) {
	std::string	  frag, vert = ""; // TODO:: Make it directly in the heap
	uint8		  current = 0;
	std::ifstream file(path);
	for(std::string line; std::getline(file, line);) {
		if(line.find("#fragment") != -1)
			current = 1;
		else if(line.find("#vertex") != -1)
			current = 2;
		if(current == 1)
			frag += line + '\n';
		else if(current == 2)
			vert += line + '\n';
	}
	file.close();
	return {_strdup(&vert[0]), _strdup(&frag[0])};
}

void Shader::_GlGen(ShaderText* src) {
	int	   c			= 0;
	uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	NW_GL_CALL(glShaderSource(vertexShader, 1, &(src->vertex), NULL));
	glCompileShader(vertexShader);
	c = CheckShaderCompileError(vertexShader, src->vertex, "SHADER::VERTEX::COMPILATION FAILED AT: ");
	if(!c) {
		_glID = 0;
		NW_GL_CALL(glDeleteShader(vertexShader));
		return;
	}
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	NW_GL_CALL(glShaderSource(fragmentShader, 1, &(src->fragment), NULL));
	NW_GL_CALL(glCompileShader(fragmentShader));
	c = CheckShaderCompileError(fragmentShader, src->fragment, "SHADER::FRAGMENT::COMPILATION FAILED AT: ");
	if(!c) {
		_glID = 0;
		NW_GL_CALL(glDeleteShader(fragmentShader));
		return;
	}
	_glID = glCreateProgram();
	NW_GL_CALL(glAttachShader(_glID, vertexShader));
	NW_GL_CALL(glAttachShader(_glID, fragmentShader));
	NW_GL_CALL(glLinkProgram(_glID));
	c = CheckShaderLinkError(_glID);
	NW_GL_CALL(glDeleteShader(vertexShader));
	NW_GL_CALL(glDeleteShader(fragmentShader));
	if(!c) {
		Delete();
	}
};

Asset* Shader::GetFromCache(void* identifier) {
	if(identifier == nullptr)
		return nullptr;
	auto iter = Shader::resList.find(*(ShaderIdentifier*)identifier);
	if(iter == Shader::resList.end())
		return nullptr;
	return &iter->second;
}

Asset* Shader::_LoadDirect(void* shaderTextPtr, void* identifier) {
	Shader* shader		= &resList.emplace(*(ShaderIdentifier*)identifier, Shader()).first->second;
	shader->_identifier = *(ShaderIdentifier*)identifier;
	shader->_GlGen((ShaderText*)shaderTextPtr);
	Move(shader);
	return shader;
}

Asset* Shader::LoadFromFile(const char* path, void* identifier) {
	parser.Reset();
	ShaderText res;
	parser.ParseFromPath(path);
	res.vertex	 = parser.GetVertTxt().c_str();
	res.fragment = parser.GetFragTxt().c_str();
	SetReflectedUniforms(parser);
	SetEnabledAtts(parser);
	return _LoadDirect(&res, identifier); // LoadFromBuffer(&res, identifier);
}

Asset* Shader::LoadFromBuffer(void* shaderTextPtr, void* identifier) {
	ShaderIdentifier* idd = (ShaderIdentifier*)identifier;
	ShaderText*		  raw = (ShaderText*)shaderTextPtr;
	ShaderText		  parsed;
	ShaderParser	  vertParser;
	vertParser.constants = parser.constants;
	vertParser.Parse(raw->vertex);
	parsed.vertex = vertParser.vert.c_str();
	parser.Parse(raw->fragment);
	parsed.fragment = parser.frag.c_str();
	SetReflectedUniforms(parser);
	SetReflectedUniforms(vertParser);
	SetEnabledAtts(parser);
	return _LoadDirect(&parsed, idd);
}

void Shader::Move(Asset* other) {
	Shader& otherS			 = *(Shader*)other;
	otherS._enabledAtts		 = std::move(_enabledAtts);
	otherS.reflectedUniforms = std::move(reflectedUniforms);
}

void Shader::SetReflectedUniforms(const ShaderParser& p) {
	for(auto pp : p.GetUniforms()) {
		reflectedUniforms.insert(pp);
	}
}

void Shader::SetEnabledAtts(const ShaderParser& p) {
	_enabledAtts.clear();
	for(uint16 i : p.enabledAtts) {
		_EnableAtt(i);
	}
}

NW_IMPL_RES_LIST(ShaderIdentifier, Shader)

void Shader::DirectUse() { NW_GL_CALL(glUseProgram(_glID)); }

void Shader::Use() {
	NW_GL_CALL(glUseProgram(_glID));
	FrameBuffer* fbo = FrameBuffer::GetCurrent();
	if(!fbo)
		return;
	for(int i = 0; i < fbo->GetAttNum(); ++i) {
		if(_enabledAtts.find(i) != _enabledAtts.end()) {
			NW_GL_CALL(glColorMaski(i, !(disablePerChannelWrts & (1 << 3)), !(disablePerChannelWrts & (1 << 2)),
									!(disablePerChannelWrts & (1 << 1)), !(disablePerChannelWrts & 1)));
		} else {
			NW_GL_CALL(glColorMaski(i, 0, 0, 0, 0));
		}
	}
}

void Shader::Unuse() {
	NW_GL_CALL(glUseProgram(0));
	FrameBuffer* fbo = FrameBuffer::GetCurrent();
	if(!fbo)
		return;
	for(int i = 0; i < fbo->GetAttNum(); ++i) {
		NW_GL_CALL(glColorMaski(i, 1, 1, 1, 1));
	}
}

void Shader::_DisableAtt(int i) { _enabledAtts.erase(i); }

void Shader::_EnableAtt(int i) { _enabledAtts[i] = i; }

void Shader::SetMat4x4(int loc, const float* value) { NW_GL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, value)); }

void Shader::SetUniform1f(int loc, float value) { NW_GL_CALL(glUniform1f(loc, value)); }

void Shader::SetUniform1i(int loc, int value) { NW_GL_CALL(glUniform1i(loc, value)); }

void Shader::SetUniform2f(int loc, float x, float y) { NW_GL_CALL(glUniform2f(loc, x, y)); }

void Shader::SetUniform2i(int loc, int x, int y) { NW_GL_CALL(glUniform2i(loc, x, y)); }

void Shader::SetVector2(int loc, float value0, float value1) { NW_GL_CALL(glUniform2f(loc, value0, value1)); }

void Shader::SetUniform3f(int loc, float x, float y, float z) { NW_GL_CALL(glUniform3f(loc, x, y, z)); }

void Shader::SetUniform3i(int loc, int x, int y, int z) { NW_GL_CALL(glUniform3i(loc, x, y, z)); }

void Shader::SetUniform4f(int loc, float x, float y, float z, float w) { NW_GL_CALL(glUniform4f(loc, x, y, z, w)); }

void Shader::SetUniform4i(int loc, int x, int y, int z, int w) { NW_GL_CALL(glUniform4i(loc, x, y, z, w)); }

void Shader::SetUniform2f(const char* name, const v2f& v) { SetUniform2f(name, v.x, v.y); };
void Shader::SetUniform2i(const char* name, const v2i& v) { SetUniform2i(name, v.x, v.y); };
void Shader::SetUniform3f(const char* name, const v3f& v) { SetUniform3f(name, v.x, v.y, v.z); };
void Shader::SetUniform3i(const char* name, const v3i& v) { SetUniform3i(name, v.x, v.y, v.z); };
void Shader::SetUniform4f(const char* name, const v4f& v) { SetUniform4f(name, v.x, v.y, v.z, v.w); };
void Shader::SetUniform4i(const char* name, const v4i& v) { SetUniform4i(name, v.x, v.y, v.z, v.w); };

void Shader::SetUniformArrayf(int loc, float* value, int size) { NW_GL_CALL(glUniform1fv(loc, size, value)); }

void Shader::SetUniformArray2f(int loc, float* value, int size) { NW_GL_CALL(glUniform2fv(loc, size, value)); }

void Shader::SetUniformArrayi(int loc, int* value, int size) { NW_GL_CALL(glUniform1iv(loc, size, value)); }

void Shader::SetUniformArray2f(const char* name, float* value, int size) {
	SetUniformArray2f(glGetUniformLocation(_glID, name), value, size);
}

void Shader::SetMat4x4(const char* name, const float* value) { SetMat4x4(glGetUniformLocation(_glID, name), value); }

void Shader::SetUniform1f(const char* name, float value) { SetUniform1f(glGetUniformLocation(_glID, name), value); }

void Shader::SetVector2(const char* name, float value0, float value1) {
	SetVector2(glGetUniformLocation(_glID, name), value0, value1);
}

void Shader::SetUniform2i(const char* name, int x, int y) { SetUniform2i(glGetUniformLocation(_glID, name), x, y); }

void Shader::SetUniform2f(const char* name, float x, float y) { SetUniform2f(glGetUniformLocation(_glID, name), x, y); }

void Shader::SetUniform1i(const char* name, const int value) { SetUniform1i(glGetUniformLocation(_glID, name), value); }

void Shader::SetUniform3f(const char* name, float x, float y, float z) {
	SetUniform3f(glGetUniformLocation(_glID, name), x, y, z);
};

void Shader::SetUniform3i(const char* name, int x, int y, int z) { SetUniform3i(glGetUniformLocation(_glID, name), x, y, z); };

void Shader::SetUniform4f(const char* name, float x, float y, float z, float w) {
	SetUniform4f(glGetUniformLocation(_glID, name), x, y, z, w);
}

void Shader::SetUniform4i(const char* name, int x, int y, int z, int w) {
	SetUniform4i(glGetUniformLocation(_glID, name), x, y, z, w);
}

void Shader::SetUniformArrayf(const char* name, float* value, int size) {
	SetUniformArrayf(glGetUniformLocation(_glID, name), value, size);
}

void Shader::SetUniformArrayi(const char* name, int* value, int size) {
	SetUniformArrayi(glGetUniformLocation(_glID, name), value, size);
}

void Shader::Delete() {
	_glID = 0;
	NW_GL_CALL(glDeleteProgram(this->_glID));
}

void Shader::Clean() {
	--_usageCounter;
	if(_usageCounter > 0)
		return;
	ShaderIdentifier id = GetIDWithAsset<Shader*, ShaderIdentifier>(this);
	EraseRes<Shader>(id);
}

int Shader::GetUniformLoc(const char* name) {
	int ret = glGetUniformLocation(_glID, name);
	return ret;
}

//------------------COMPUTE SHADER------------------

void ComputeShader::_GlGen(ComputeShaderText* src2) {
	ComputeShaderText src  = *src2;
	int				  comp = glCreateShader(GL_COMPUTE_SHADER);
	NW_GL_CALL(glShaderSource(comp, 1, &src, NULL));
	NW_GL_CALL(glCompileShader(comp));
	CheckShaderCompileError(comp, src, "SHADER::COMPUTE::COMPILATION FAILED AT: ");
	_glID = NW_GL_CALL(glCreateProgram());
	NW_GL_CALL(glAttachShader(_glID, comp));
	NW_GL_CALL(glLinkProgram(_glID));
	bool c = CheckShaderLinkError(_glID);
	NW_GL_CALL(glDeleteShader(comp));
	if(!c) {
		Delete();
	}
}

Asset* ComputeShader::GetFromCache(void* identifier) {
	if(identifier == nullptr)
		return nullptr;
	auto iter = ComputeShader::resList.find(*(ComputeShaderIdentifier*)identifier);
	if(iter == ComputeShader::resList.end())
		return nullptr;
	return &iter->second;
}

void ComputeShader::SetReflectedUniforms(const ShaderParser& p) {
	for(auto pp : p.GetUniforms()) {
		reflectedUniforms.insert(pp);
	}
}

Asset* ComputeShader::LoadFromFile(const char* path, void* identifier) {
	parser.Reset();
	ComputeShaderText res;
	parser.ParseFromPath(path);
	res = parser.GetComputeTxt().c_str(); // TODO::
	SetReflectedUniforms(parser);
	return _LoadDirect(&res, identifier); // LoadFromBuffer(&res, identifier);
}

Asset* ComputeShader::LoadFromBuffer(void* shaderTextPtr, void* identifier) {
	ComputeShaderIdentifier* idd = (ComputeShaderIdentifier*)identifier;
	ComputeShaderText*		 raw = (ComputeShaderText*)shaderTextPtr;
	ComputeShaderText		 parsed;
	parser.Parse(*raw);
	parsed = parser.frag.c_str();
	SetReflectedUniforms(parser);
	return _LoadDirect(&parsed, idd);
}

Asset* ComputeShader::_LoadDirect(void* shaderTextPtr, void* identifier) {
	ComputeShader* shader = &resList.emplace(*(ComputeShaderIdentifier*)identifier, ComputeShader()).first->second;
	shader->_identifier	  = *(ComputeShaderIdentifier*)identifier;
	shader->_GlGen((ComputeShaderText*)shaderTextPtr);
	Move(shader);
	return shader;
}

void ComputeShader::Move(Asset* other) {
	ComputeShader& otherS	 = *(ComputeShader*)other;
	otherS.reflectedUniforms = std::move(reflectedUniforms);
}

void ComputeShader::Delete() {
	_glID = 0;
	NW_GL_CALL(glDeleteProgram(_glID));
}

void ComputeShader::Clean() {
	--_usageCounter;
	if(_usageCounter > 0)
		return;
	ComputeShaderIdentifier id = GetIDWithAsset<ComputeShader*, ComputeShaderIdentifier>(this);
	EraseRes<ComputeShader>(id);
}

void ComputeShader::Dispatch(const iVec3& s) { NW_GL_CALL(glDispatchCompute(s.x, s.y, s.z)); }

void ComputeShader::Dispatch() { NW_GL_CALL(glDispatchCompute(_dispatchSize.x, _dispatchSize.y, _dispatchSize.z)); }

void ComputeShader::SetDispatchSize(const iVec3& s) { _dispatchSize = s; }

void ComputeShader::BindBuffer(int buffer, int slot) { NW_GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, buffer)); }

ShaderParser ComputeShader::parser;

NW_IMPL_RES_LIST(ComputeShaderIdentifier, ComputeShader)
