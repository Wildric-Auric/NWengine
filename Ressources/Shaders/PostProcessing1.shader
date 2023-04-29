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

//fragment shader
#version 330 core

uniform float uTime;
uniform sampler2D uTex0;
uniform vec2 uResolution;
uniform vec2 uMouse;
in vec2 uv;
in vec4 screenPos;

out vec4 FragColor;

void main(){
    vec4 col = texture(uTex0, uv);
    if (col.a < 0.1) discard;
    if (uv.y > 0.5)
	col = vec4(1.0 - col.xyz, col.a);
    FragColor = col;
}
