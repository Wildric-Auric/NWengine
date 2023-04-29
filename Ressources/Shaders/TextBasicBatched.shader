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
                      (float((0x000003FF & floatBitsToInt(usrData.x))))       / MAXUINT10,
                      (float((0x000FFC00 & floatBitsToInt(usrData.y))>>0xA))  / MAXUINT10,
                      (float((0x000003FF & floatBitsToInt(usrData.y))))       / MAXUINT10
                     );
};
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