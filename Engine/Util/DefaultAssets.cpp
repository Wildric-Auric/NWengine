#include "DefaultAssets.h"
const char* ShaderDefVert =
	R"V0G0N( 
        #pragma vertex
        #version 330 core
        layout(location = 0) in vec3 attribPos;
        layout(location = 1) in vec2 texCoord;
        
        uniform mat4 uMvp = mat4(1.0);
        
        out vec2 uv;
        
        void main() {
            gl_Position = uMvp * vec4(attribPos, 1.0);
            uv = texCoord;
        };
)V0G0N";
const char* ShaderDefVerBatched =
	R"V0G0N( 
        #pragma vertex 
        #version 330 core
        layout(location = 0) in vec3  attribPos;
        layout(location = 1) in vec2  texCoord;
        layout(location = 2) in vec3  usrData;
        layout(location = 3) in float sampleIDattrib;
        out vec2  uv;
        out float sampleID;
        void main() {
            gl_Position = vec4(attribPos, 1.0);
            uv = texCoord;
            sampleID  = sampleIDattrib;
        };
)V0G0N";

const ShaderText ShaderTexturedDefaultStr = {ShaderDefVert,
											 R"V0G0N(        
        #pragma fragment
        #version 330 core
        uniform sampler2D uTex0;
        in vec2 uv;
        out vec4 FragColor;
        void main() {
            vec4 col = texture(uTex0, uv);
            FragColor = col;
        };
)V0G0N"};

const ShaderText ShaderTexturedBatchedDefaultStr = {ShaderDefVerBatched,
													R"V0G0N(
        #pragma fragment 
        #version 330 core
        #pragma def MAX_TEX MaxTexNum

        uniform sampler2D uTex[MAX_TEX];
        in vec2  uv;
        in float sampleID;
        out vec4 FragColor;
        
        void main() {
            int texID  = int(sampleID);
            vec4 col   = texture(uTex[texID], uv);
            FragColor  = col;
        }
)V0G0N"};

const ShaderText ShaderTextDefaultStr = {ShaderDefVert,
										 R"V0G0N(
        #pragma fragment 
        #version 330 core
        uniform sampler2D uTex0;
        in vec2 uv;
        out vec4 FragColor;
        uniform vec4 uCol = vec4(1.0);
        uniform int  uForceNoAlpha = 0;
        void main(){
            float val = texture(uTex0, vec2(uv.x, 1.0 - uv.y)).x * uCol.a;
            vec4 col = vec4(uCol.xyz, uForceNoAlpha != 0 ? 1.0 : val);
            FragColor = col;
        }
)V0G0N"};

const ShaderText ShaderTextBatchedStr = {
	R"V0G0N( 
        #pragma vertex
        #version 330 core
        #define MAXUINT10 1023.0

        layout(location = 0) in vec3  attribPos;
        layout(location = 1) in vec2  texCoord;
        layout(location = 2) in vec3  usrData;
        layout(location = 3) in float sampleIDattrib;

        out vec2  uv;
        out vec4  color10bit;
        out float sampleID;

        void main() {
            gl_Position = vec4(attribPos, 1.0);
            uv = texCoord;
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
        #pragma fragment 
        #version 330 core
        #pragma def MAX_TEX MaxTexNum

        uniform sampler2D uTex[MAX_TEX];
        in vec2  uv;
        in vec4  color10bit;
        in float sampleID;
        out vec4 FragColor;

        vec3 color = vec3(1.0);
        void main() {
            vec4 col  = vec4(color10bit.xyz, texture(uTex[int(sampleID)], vec2(uv.x, 1.0 - uv.y)).x * color10bit.a);
            FragColor = col;
        }
)V0G0N"};

const ShaderText ShaderTexturedNoAlphaStr{ShaderDefVert,
										  R"V0G0N(        
        #pragma fragment
        #version 330 core
        uniform sampler2D uTex0;
        in vec2 uv;
        out vec4 FragColor;
        void main() {
            vec3 col  = texture(uTex0, uv).xyz;
            FragColor = vec4(col,1.0);
        };
)V0G0N"};

#define W	  255, 255, 255
#define WA	  255, 255, 255, 255
#define R	  255, 0, 0
#define ALPHA false

/*Pay attention to size as the spec says :
	"Specifies the width of the texture image.
	All implementations support texture images that are at least 1024 texels wide."
*/
static uint8 imageDefaultBuff[] = {
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W};

const Image ImageDefault{std::string(), imageDefaultBuff, 3, 16, 16, ALPHA};

TextureIdentifier TextureDefaultID				 = {"DefaultTexture", ALPHA};
ShaderIdentifier  ShaderTexturedDefaultID		 = "DefaultShaderTextured";
ShaderIdentifier  ShaderTexturedColoredDefaultID = "DefaultShaderColoredTextured";
ShaderIdentifier  ShaderTexturedBatchedDefaultID = "DefaultShaderTexturedBatched";
ShaderIdentifier  ShaderTextDefaultID			 = "DefaultShaderText";
ShaderIdentifier  ShaderTextBatchedDefaultID	 = "DefaultShaderTextBatched";
ShaderIdentifier  ShaderTriangleDefaultID		 = "DefaultShaderTriangle";
ShaderIdentifier  ShaderCircleDefaultID			 = "DefaultShaderCircle";
ShaderIdentifier  ShaderTexturedNoAlphaID		 = "DefaultShaderTexturedNoAlphaID";

const ShaderText ShaderCircleDefaultStr = {ShaderDefVert,
										   R"V0G0N(        
        #pragma fragment
        #version 330 core 
        uniform sampler2D uTex0;
        uniform float uAA = 0.0;
        in vec2 uv;
        
        out vec4 FragColor;
        
        void main() {
            vec2 c = uv - vec2(0.5,0.5);
            float p = c.x * c.x + c.y * c.y; 
            float aaval = 1.0 - smoothstep(0.25-uAA,0.25,p);
            vec4 col = texture(uTex0,uv) * aaval; 
            FragColor = col;
        } 
)V0G0N"};

const ShaderText ShaderTriangleDefaultStr = {
	R"V0G0N( 
        #pragma vertex
        #version 330 core
        layout(location = 0) in vec3 attribPos;
        layout(location = 1) in vec2 texCoord;
        uniform vec2 uVert[3];
        uniform mat4 uMvp = mat4(1.0); 
        out vec2 uv;
        void main() {
            gl_Position = uMvp * vec4(uVert[gl_VertexID], attribPos.z, 1.0);
            uv = texCoord;
        }
)V0G0N",
	R"V0G0N(        
        #pragma fragment
        #version 330 core
        
        uniform sampler2D uTex0;
        in vec2 uv; 
        out vec4 FragColor;
        
        void main() {
            vec4 col = texture(uTex0, uv);
            FragColor = col;
        }
        
)V0G0N"};

const ShaderText ShaderTexturedColoredDefaultStr = {ShaderDefVert,
													R"V0G0N(        
        #pragma fragment
        #version 330 core
        uniform sampler2D  uTex0;
        uniform vec4 uCol = vec4(1.0);
        in vec2 uv;
        out vec4 FragColor;
        void main() {
            vec4 col = texture(uTex0, uv);
            FragColor = col * uCol;
        };
)V0G0N"};

//---------------------------
const ShaderText ShaderUIWindowStr{ShaderDefVert,
								   R"V0G0N(        
        #pragma fragment
        #version 330 core
        uniform sampler2D uTex0;
        uniform vec2 uRes;
        uniform float uTitleHeight = 20.0;
        uniform float uBorderWidth = 1.0;
        //----Colors----
        uniform vec4  uCol       = vec4(1.0);
        uniform vec4  uBarCol    = vec4(1.0,0.0,0.0,1.0);
        uniform vec4  uBorderCol = vec4(vec3(0.0),1.0);
        //------------
        in vec2 uv;
        out vec4 FragColor;
        void main() {
            vec4 col = texture(uTex0, uv);
            bool ycond = (1.0-uv.y)*uRes.y < uTitleHeight;
            bool ycondBorder = (1.0-uv.y)*uRes.y <= uBorderWidth || uv.y * uRes.y <= uBorderWidth;
            bool xcondBorder = (1.0-uv.x)*uRes.x <= uBorderWidth || uv.x * uRes.x <= uBorderWidth;
            bool condBorder = ycondBorder || xcondBorder;
            vec4 color = ycond ? uBarCol               : uCol;
            color = condBorder ? uBorderCol            : color;
            FragColor = color;
        };
)V0G0N"};
ShaderIdentifier ShaderUIWindowID = "DefaultShaderUIWindowID";

const ShaderText ShaderUISliderStr{ShaderDefVert,
								   R"V0G0N(        
        #pragma fragment
        #version 330 core
        uniform sampler2D uTex0;
        uniform vec2 uRes;
        uniform float uPosX  = 0.0;
        uniform vec4  uCol   = vec4(vec3(0.0),1.0);
        in vec2 uv;
        out vec4 FragColor;
        void main() {
            vec2 coord = uRes * (uv-0.5);
            vec4 color = uCol;
            if (abs(coord.y) > 2 && abs(coord.x + uPosX) > 5) 
                color.a = 0.0;
            FragColor = color;
        };
)V0G0N"};

ShaderIdentifier ShaderUISliderID = "DefaultShaderUISliderID";

const ShaderText ShaderUICheckboxStr{ShaderDefVert,
									 R"V0G0N(        
        #pragma fragment
        #version 330 core
        uniform sampler2D uTex0;
        uniform vec2 uRes;
        uniform int  uState = 0;
        uniform vec4 uBgCol = vec4(vec3(0.0),1.0);
        uniform vec4 uFgCol = vec4(vec3(1.0),1.0);
        in vec2 uv;
        out vec4 FragColor;
        void main() {
            vec2 coord = uv - 0.5;
            vec4 color = uBgCol;
            if (uState == 1 && length(coord) < 0.25) {
                color = uFgCol;
            }
            FragColor = color;
        };
)V0G0N"};

ShaderIdentifier ShaderUICheckboxID = "DefaultShaderUICheckboxID";
