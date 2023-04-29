//vertex shader
#version 330 core

layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 uMvp = mat4(1.0);

out vec2 uv;
out vec4 screenPos;

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv = texCoord;
    screenPos = gl_Position;
};

//fragment shader
#version 330 core

uniform sampler2D uTex0;
uniform float     uAlpha = 1.0;

in vec2 uv;
in vec4 screenPos;

out vec4 FragColor;

void main() {
    vec4 sampled = texture(uTex0, uv);
    FragColor = vec4(sampled.xyz, sampled.w * uAlpha);
}



