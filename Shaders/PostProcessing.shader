//vertex shader
#version 330 core

layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 uMvp = mat4(1.0);
uniform vec2 uResolution;
uniform vec2 uMouse;

out vec2 uv;
out vec4 screenPos;

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv = texCoord;
    screenPos = gl_Position;
};

//fragment shader
#version 330 core

float strength = 0.5;
float depth = 0.5;

uniform float uTime;
uniform sampler2D uTex0;
uniform vec2 uResolution;
uniform vec2 uMouse;

in vec2 uv;
in vec4 screenPos;
out vec4 FragColor;

void main() {
    vec2 uv1 = uv * 50.0;
    vec2 intPart = vec2(floor(uv1.x), floor(uv1.y));
    vec2 floatPart = vec2(fract(uv1.x), fract(uv1.y));
    vec4 col = texture(uTex0, uv);
    if (floatPart.x > depth || floatPart.y > depth) {
        col = mix(vec4(0.0, 0.0, 0.0, 1.0), col, strength);
    };
    FragColor = col;
}
