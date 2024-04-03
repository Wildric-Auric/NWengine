#include "DefaultAssets.h"

const ShaderText ShaderTexturedDefaultStr = {
R"V0G0N( 
        //vertex shader
        #version 330 core
        
        layout(location = 0) in vec3 attribPos;
        layout(location = 1) in vec2 texCoord;
        
        uniform mat4 uMvp = mat4(1.0);
        uniform vec2 uResolution;
        
        out vec2 uv;
        out vec4 screenPos;
        
        void main() {
            gl_Position = uMvp * vec4(attribPos, 1.0);
            uv = texCoord;
            screenPos = gl_Position;
        };
)V0G0N",
R"V0G0N(        
        //fragment shader
        #version 330 core
        
        uniform float uTime;
        uniform sampler2D uTex0;
        uniform vec2 uResolution;
        uniform vec2 uMouse;
        in vec2 uv;
        in vec4 screenPos;
        
        out vec4 FragColor;
        
        void main() {
            vec4 col = texture(uTex0, uv);
            if (col.a < 0.1) discard;
            FragColor = col;
        };
        
)V0G0N"
};

const ShaderText ShaderTexturedBatchedDefaultStr = {
R"V0G0N( 
        //vertex shader
        #version 330 core
        
        layout(location = 0) in vec3  attribPos;
        layout(location = 1) in vec2  texCoord;
        layout(location = 2) in vec3  usrData;
        layout(location = 3) in float sampleIDattrib;
        
        out vec2  uv;
        out vec4  screenPos;
        out float sampleID;
        out vec3  test;
        
        void main() {
            gl_Position = vec4(attribPos, 1.0);
            uv = texCoord;
            screenPos = gl_Position;
            sampleID  = sampleIDattrib;
            test      = usrData;
        };
)V0G0N",
R"V0G0N(
        //fragment shader
        #version 330 core
        
        uniform sampler2D uTex[32];
        in vec2  uv;
        in vec4  screenPos;
        in vec3  test;
        in float sampleID;
        out vec4 FragColor;
        
        void main() {
            int texID  = int(sampleID);
            vec4 col   = texture(uTex[texID], uv);
            FragColor  = col;
        }
)V0G0N"
};

const ShaderText ShaderTextDefaultStr = {R"V0G0N( 
        //vertex shader
        #version 330 core

        layout (location = 0) in vec3 attribPos;
        layout(location = 1) in vec2 texCoord;

        uniform mat4 uMvp = mat4(1.0);
        uniform vec2 uResolution;

        out vec2 uv;
        out vec4 screenPos;

        void main() {
            gl_Position = uMvp * vec4(attribPos, 1.0);
            uv = texCoord;
            screenPos = gl_Position;
        };
)V0G0N",
R"V0G0N(
        //fragment shader
        #version 330 core

        uniform sampler2D uTex0;
        in vec2 uv;
        in vec4 screenPos;

        out vec4 FragColor;

        vec3 color = vec3(1.0);

        void main(){
            vec4 col = vec4(color,texture(uTex0, vec2(uv.x, 1.0 - uv.y)).x);
            FragColor = col;
        }
)V0G0N"
};

const ShaderText ShaderTextBatchedStr = {
    R"V0G0N( 
        //vertex shader
        #version 330 core
        #define MAXUINT10 1023.0

        layout(location = 0) in vec3  attribPos;
        layout(location = 1) in vec2  texCoord;
        layout(location = 2) in vec3  usrData;
        layout(location = 3) in float sampleIDattrib;

        out vec2  uv;
        out vec4  screenPos;
        out vec4  color10bit;
        out float sampleID;

        void main() {
            gl_Position = vec4(attribPos, 1.0);
            uv = texCoord;
            screenPos = gl_Position;
            sampleID  = sampleIDattrib;
            //floatBitsToInt(var) = *(int*)&var in C syntax
            color10bit = vec4((float((0x000FFC00 & floatBitsToInt(usrData.x)) >>0xA) ) / MAXUINT10,
                              (float((0x000003FF & floatBitsToInt(usrData.x))))        / MAXUINT10,
                              (float((0x000FFC00 & floatBitsToInt(usrData.y))>>0xA))   / MAXUINT10,
                              (float((0x000003FF & floatBitsToInt(usrData.y))))        / MAXUINT10
                             );
        };
)V0G0N",
R"V0G0N(
        //fragment shader
        #version 330 core

        uniform sampler2D uTex[32];
        in vec2  uv;
        in vec4  screenPos;
        in vec4  color10bit;
        in float sampleID;
        out vec4 FragColor;

        vec3 color = vec3(1.0);
        void main() {
            vec4 col  = vec4(color10bit.xyz, texture(uTex[int(sampleID)], vec2(uv.x, 1.0 - uv.y)).x * color10bit.a);
            FragColor = col;
        }
)V0G0N"
};


#define W  255,255,255
#define WA 255,255,255,255
#define R  255,0,0
#define ALPHA  false

/*Pay attention to size as the spec says :
    "Specifies the width of the texture image. 
    All implementations support texture images that are at least 1024 texels wide."
*/
static uint8 imageDefaultBuff[] =  { 
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
     R,R,R,R,R,R,R,R,R,W,W,W,W,W,W,W
};

const Image ImageDefault {
    std::string(),
    imageDefaultBuff,
    3,
    16, 
    16,
    ALPHA
};


TextureIdentifier TextureDefaultID                  = { "DefaultTexture", ALPHA };
ShaderIdentifier  ShaderTexturedDefaultID           = "DefaultShaderTextured";

ShaderIdentifier  ShaderTexturedBatchedDefaultID    = "DefaultShaderTexturedBatched";
ShaderIdentifier  ShaderTextDefaultID               = "DefaultShaderText";
ShaderIdentifier  ShaderTextBatchedDefaultID        = "DefaultShaderTextBatched";