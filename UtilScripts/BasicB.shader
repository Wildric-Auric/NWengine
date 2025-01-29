#pragma vertex
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

#pragma fragment 
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
