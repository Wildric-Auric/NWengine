//vertex shader
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

//fragment shader
#version 330 core
#define pi 3.14159
uniform float uTime;
uniform sampler2D uTex0;
in vec2 uv;
out vec4 FragColor;
void main(){ 
    vec2 center = vec2(0.5, 0.5);
    float dis = distance(uv, center);
    float factor = 0.1 * sin(uTime * .3);
    float formula = factor / (dis);
   //vec4 col = vec4(vec3(formula), 1.0);
    vec4 col = texture(uTex0, uv);
    //col.b += .1 / dis;
    // Output to screen
    FragColor = col;
}





