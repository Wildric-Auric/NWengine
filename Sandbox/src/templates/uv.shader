#pragma vertex
#version 330 core

layout (location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 uMvp = mat4(1.0);
uniform vec2 uResolution;

out vec2 uv;

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv = texCoord;
};

#pragma fragment
#version 330 core

uniform sampler2D uTex0;
in vec2 uv;

out vec4 FragColor;

void main(){
    FragColor = vec4(uv.x,0.0,uv.y,1.0);
}





