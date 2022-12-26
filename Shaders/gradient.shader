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

uniform float uTime;
uniform sampler2D uTex0;
uniform vec2 uResolution;
uniform vec2 uMouse;

in vec2 uv;
in vec4 screenPos;
out vec4 FragColor;


#define NEIGHBOURS           2
#define EXP                  REP(0,0,4,4) vec3(0.0)

#define TEX   uTex0
#define UV    uv
#define PIXEL pixel

#define REP0(X)            
#define REP1(X) 		   texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP2(X) REP1(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP3(X) REP2(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP4(X) REP3(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP5(X) REP4(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP6(X) REP5(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP7(X) REP6(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP8(X) REP7(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP9(X) REP8(X-1)  texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +
#define REP10(X) REP9(X-1) texture(TEX, UV + PIXEL * vec2(float((X-1)%NEIGHBOURS),float((X-1)/NEIGHBOURS))).xyz +

#define REP(HUNDREDS,TENS,ONES,X) \
  REP##HUNDREDS(REP10(REP10(X))) \
  REP##TENS(REP10(X)) \
  REP##ONES(X)

vec3 blur()
{
    vec2 pixel = 1.0 / uResolution.xy;
    vec3 col = vec3(0.0);
    col += (1.0 / float(NEIGHBOURS * NEIGHBOURS)) * EXP;
    return col;
}


void main() {
    vec4 col = vec4(blur(),1.0);
    FragColor = col;
}





