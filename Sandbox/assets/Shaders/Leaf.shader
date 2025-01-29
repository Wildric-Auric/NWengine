#pragma vertex
#version 330 core
        
layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;
layout(location = 3) in vec3 usrData;

        
uniform mat4 uMvp = mat4(1.0);
uniform vec2 uResolution;
        
out vec2 uv;
out vec4 screenPos;
flat out float color;

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv = texCoord;
    screenPos = gl_Position;
    color = usrData.x;
};
#pragma fragment
#version 330 core

uniform sampler2D uTex0;
flat in float color;
in vec2 uv;
out vec4 FragColor;

void main() {
    vec4 col  = vec4(0.4, 0.6, 0.1, 1.0);
    FragColor = col;
};