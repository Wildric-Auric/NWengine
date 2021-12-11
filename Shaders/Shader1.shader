//vertex shader
#version 330 core

layout (location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;
uniform mat4 projectionMat = mat4(1.0);
uniform vec2 uResolution;
uniform vec2 uTranslation; //TODO: Improve this, it's a temporary solution
out vec2 uv;
void main() {
    vec3 newPos = attribPos + vec3(uTranslation,0.0);
    gl_Position = projectionMat * vec4(newPos, 1.0);
    uv = texCoord;
};

//fragment shader
#version 330 core
#define pi 3.14159
uniform float uTime;
in vec2 uv;
out vec4 FragColor;
void main(){ 
    vec2 center = vec2(0.5, 0.5);
    float dis = distance(uv, center);
    float factor = 0.1 * sin(uTime * .3);
    float formula = factor / (dis);
    vec3 col = vec3(formula);
    col.b += .1 / dis;
    // Output to screen
    FragColor = vec4(col, 1.0);
}





