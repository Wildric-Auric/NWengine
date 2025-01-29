#pragma vertex
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

#pragma fragment
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





