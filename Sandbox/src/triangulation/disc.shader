#pragma vertex
#version 330 core
layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 uMvp = mat4(1.0);

out vec2 uv;

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv = texCoord;
};

#pragma fragment
#version 330 core 
uniform sampler2D uTex0;
uniform float uAA = 0.0;
in vec2 uv;

out vec4 FragColor;

void main() {
    vec2 c = uv - vec2(0.5,0.5);
    float p = c.x * c.x + c.y * c.y; 
    float aaval = 1.0 - smoothstep(0.25-uAA,0.25,p);
    aaval *= float((p > 0.24));
    vec4 col = texture(uTex0,uv) * aaval; 
    FragColor = col;
} 
