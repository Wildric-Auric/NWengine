//vertex shader
#version 330 core

layout(location = 0) in vec3  attribPos;
layout(location = 1) in vec2  texCoord;
layout(location = 2) in float sampleIDattrib;

out vec2 uv;
out vec4 screenPos;
out float sampleID;

void main() {
    gl_Position = vec4(attribPos, 1.0);
    uv = texCoord;
    screenPos = gl_Position;
    sampleID  = sampleIDattrib;
};

//fragment shader
#version 330 core

uniform sampler2D uTex[32];
in vec2  uv;
in vec4  screenPos;
in float sampleID;
out vec4 FragColor;

vec3 color = vec3(1.0);
void main() {
    vec4 col = vec4(  color , max ( pow (1.0 - length(uv - 0.5) / 1.414, 10.0 ), 0.0 ) );
    FragColor = col;
}