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
in vec2 uv;
in vec4 screenPos;

out vec4 FragColor;

const float ZCoord = 0.6;
const float norm = 0.35; // ZCoord as max value
const float lensRadius = 1.2;
const float blur = 3.;

void main() {
    vec2 mouse = uMouse / uResolution;
    vec3 uv1 = vec3(uv, 0.0);
    vec4 tex = textureLod(uTex0, uv1.xy, blur);
    vec3 sphereCenter = vec3(mouse.xy, ZCoord);
    vec3 tempVector = uv1 - sphereCenter;
    float len = length(tempVector);
    vec3 new = (tempVector / len) * norm;
    uv1 += -(float(ZCoord / abs(ZCoord))) * new;
    if (distance(mouse, uv.xy) < lensRadius)
    {
        tex = texture(uTex0, uv1.xy);
    }

    FragColor = tex;
}
