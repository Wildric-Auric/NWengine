//vertex shader
#version 330 core

layout(location = 0) in vec3 attribPos;

uniform mat4 uMvp = mat4(1.0);

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
};

//fragment shader
#version 330 core

uniform vec3 uColor;
uniform float uAlpha;
out vec4 FragColor;

void main() {
    FragColor = vec4(uColor, uAlpha);
};




