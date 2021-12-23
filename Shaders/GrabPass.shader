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
uniform sampler2D uTex0;
uniform vec2 uResolution;
uniform vec2 uMouse;
uniform float uTime;
in vec2 uv;
in vec4 screenPos;

out vec4 FragColor;


float random(in vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898, 78.233))) *  43758.5453123);
}

float start = 0.4;
float lod = 2.0f;
void main() {
    vec2 mouse = uMouse / uResolution;
    vec2 uv1 = uv;
    vec4 col;
    vec4 waterColor = vec4(1.0);
    vec3 additional = vec3(0.0);
    float detail = 0.0;
    if (uv.y < start) {
        uv1.y = 2*start - uv.y ;
        detail = lod;
        float sign = 1.;
        int temp = int(random(uv)*10.);
        if ( temp % 2 == 0) sign = -1.;
        uv1.y = uv1.y + sin(1. / (uv.y) + uTime*4.0) * 0.01;
        uv1.x += sign * (random(uv) /100.) * sin(uTime/2.0);
        additional.b += uv.y / 0.38 - 1.0;
        waterColor = vec4(0.3, 0.45, .6, 1.0);
        //col = mix(textureLod(uTex0, uv1, detail), textureLod(uTex0,uv, 0.0), 0.3)* waterColor; //Maybe add magnifying effect for object underwater

    }
    col = textureLod(uTex0, uv1, detail) * waterColor + vec4(additional,0.0);
    FragColor = col ;
}
