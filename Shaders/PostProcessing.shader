//vertex shader
#version 330 core

layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 uMvp = mat4(1.0);

out vec2 uv0;
out vec4 screenPos;

void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv0 = texCoord;
    screenPos = gl_Position;
};

//fragment shader
#version 330 core

uniform sampler2D uTex0;

in vec2 uv0;
in vec4 screenPos;
out vec4 FragColor;

#define grain
#define vignette
//#define pseudoVintage
#define vignetteColor vec3(0.5)
#define vignetteBlur 0.1
#define vignetteRadius 0.09


float noise(vec2 p) {
    return fract(sin(dot(p, vec2(1.0, 113.0))) * 43758.5453123);
}

vec2 curve(vec2 uv)
{
    uv = (uv - 0.5) * 2.0;
    uv *= 1.1;
    uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
    uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
    uv = (uv / 2.0) + 0.5;
    uv = uv * 0.92 + 0.04;
    return uv;
}


void main() {
  

    vec2 uv = curve(uv0);
    float left = 0.0;
    float right = 1.0;
    float up = 1.0;
    float down = 0.0;
    vec2 blur = vec2(0.001, 0.001);

    vec3 col = vec3(smoothstep(left, left + blur.x, uv.x));
    col *= vec3(1.0 - smoothstep(right - blur.x, right, uv.x));
    col *= vec3(smoothstep(down, down + blur.y, uv.y));
    col *= vec3(1.0 - smoothstep(up - blur.y, up, uv.y));
    col = (1.0 - col);


    if (uv.x > left && uv.x < right && uv.y <up && uv.y > down) {
        vec2 center = vec2((left + right) * 0.5, (up + down) * 0.5);
        col = texture(uTex0, vec2((uv.x - left) / (right - left), (uv.y - down) / (up - down))).xyz;

#ifdef vignette
        float m = pow(pow(uv.x - center.x, 2.0) + pow(uv.y - center.y, 2.0), 0.5);
        float d = distance(vec2(left, up), vec2(center.x, center.y)) - vignetteRadius;
        col *= (1.0 - smoothstep(d - vignetteBlur, d, m) * vignetteColor);
#endif

#ifdef grain 
        col = mix(col, vec3(noise(uv)), 0.1);
#endif
#ifdef pseudoVintage
        col = mix(col, texture(uTex0, vec2((uv.x - left) / (right - left), (uv.y - down) / (up - down))).xyz, 0.15);
#endif


    }
    else col = vec3(uv.x * uv.y + 0.7);

    if (uv.x > 1.0 || uv.x < 0.0 || uv.y > 1.0 || uv.y < 0.0)
        col = vec3(0.0, 0.0, 0.0);

    FragColor = vec4(col, 1.0);
}
