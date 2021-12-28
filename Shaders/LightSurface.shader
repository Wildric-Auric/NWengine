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

vec4 lightColor[2] = {
                        vec4(217.0 / 255.0, 119.0 / 255.0, 205.0 / 255.0, 1.0),
                        vec4(145./255.0, 15./255.0, 41./255.0, 1.0)
};
vec2 pos[2] = {
    vec2(0.5,0.5),
    vec2(0.7,0.7),
};
void main() {
    vec2 mouse = uMouse / uResolution.xy;
    float dis = distance(uv, mouse);
    float a = 0.3; //Linear interpolation with a radius of a screen coordinates
    float ambientLight = 0.9;
    float mask = 1.0-min(dis / a, ambientLight);
    vec4 col = vec4(0.0);
    for (int i = 0; i < 2; i++) {
         col+= vec4(vec3(0.08/ pow(distance(pos[i], uv), .6)), 1.0) * lightColor[i];
    }
    col += vec4(vec3(0.03 / pow(distance(mouse, uv), 0.9)), 1.0) * vec4(0.858, 0, 0.286,1.0);
    col.a = 1.0;
    vec4 tex = texture(uTex0, uv);
    tex *= col;

    //vec4 col = vec4(mask, mask, mask, 1.0) * tex;
    FragColor = tex;
}
