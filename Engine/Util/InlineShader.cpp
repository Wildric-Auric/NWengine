#include "InlineShader.h"

uint32 InlineShader::_autoIncID = 1;

const ShaderText Raw = {
R"V0G0N( 
        #pragma vertex
        #version 330 core
        #pragma def USR_BEG INLINE_VERT_BEG_USR
        #pragma def USR_MAIN INLINE_VERT_MAIN_USR
        #pragma def USR_OUT  INLINE_VERT_OUT_USR
        layout(location = 0) in vec3 attribPos;
        layout(location = 1) in vec2 texCoord; 
        uniform mat4 uMvp = mat4(1.0); 
        out vec2 uv; 
        USR_BEG;
        void main() {
            uv = texCoord;
            USR_MAIN;
            gl_Position = USR_OUT;
        };
)V0G0N",
R"V0G0N(        
        #pragma fragment
        #version 330 core
        #pragma def USR_BEG  INLINE_FRAG_BEG_USR
        #pragma def USR_MAIN INLINE_FRAG_MAIN_USR
        #pragma def USR_OUT  INLINE_FRAG_OUT_USR 
        uniform sampler2D uTex0;
        in vec2 uv;
        out vec4 FragColor;
        USR_BEG;
        void main() {
            vec4 col = texture(uTex0, uv);
            USR_MAIN;
            FragColor = USR_OUT;
        };        
)V0G0N"

};

        
void InlineShader::AppFragGlobal(const char* txt) {
    _data.frag += std::string(txt) + ";";
}

void  InlineShader::AppFragMain(const char* txt) {
    _end.frag += std::string(txt) + ";";
}

void  InlineShader::AppVertGlobal(const char* txt) {
    _data.vert += std::string(txt) + ";";
}

void  InlineShader::AppVertMain(const char* txt) {
    _end.frag += std::string(txt) + ";";
}

void InlineShader::SetFragOut(const char* txt) {
   _fragOut = txt; 
}

void InlineShader::SetVertOut(const char* txt) {
    _vertOut = txt;
}

void InlineShader::Generate() {
    auto consts = Shader::parser.constants;

    Shader::parser.AddCnst("INLINE_VERT_BEG_USR", _data.vert.c_str());
    Shader::parser.AddCnst("INLINE_VERT_MAIN_USR", _end.vert.c_str());
    Shader::parser.AddCnst("INLINE_VERT_OUT_USR", _vertOut.c_str());

    Shader::parser.AddCnst("INLINE_FRAG_BEG_USR", _data.frag.c_str());
    Shader::parser.AddCnst("INLINE_FRAG_MAIN_USR", _end.frag.c_str());
    Shader::parser.AddCnst("INLINE_FRAG_OUT_USR", _fragOut.c_str());
    _id += std::to_string(_autoIncID++);
    auto loader = Loader<Shader>();
    _shader = loader.LoadFromBuffer((void*)&Raw,&_id);

    Shader::parser.constants = consts;
}

Shader* InlineShader::GetShader() {
    return _shader;
}

void InlineShader::SetName(const char* txt) {
    _id = txt;
}
