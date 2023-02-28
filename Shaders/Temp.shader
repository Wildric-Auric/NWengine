//vertex shader
#version 330 core
precision highp float;
layout (location = 0) in vec3 attribPos;
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


float uPerlin = 1.0;

in vec2 uv;
in vec4 screenPos;
out vec4 FragColor;


float rand(vec2 p) {
   return fract(sin(dot(p, vec2(1.0,113.0)))*43758.5453123);
}

vec3 perlin(vec2 uv0, float freq) {
    uv0 *= freq;
    vec2 intPart   = vec2(floor(uv0).x, floor(uv0).y);
    vec2 fracPart =  fract(uv0);
    fracPart      =  smoothstep(0.0, 1.0, fracPart);
    float x0       = rand(intPart);
    float x1       = rand(intPart+vec2(1,0));

    float y0       = rand(intPart+vec2(0,1));
    float y1       = rand(intPart+vec2(1,1));


    float x = mix(x0, x1, fracPart.x);
    float y = mix(y0, y1, fracPart.x);
    float xy= mix(x, y,  fracPart.y);

    return vec3(xy);
}

void main(){ 
    vec4 col = vec4(vec3(0.0,1.0,0.0), 1.0);
    FragColor = col;
}
