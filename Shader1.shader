//vertex shader
#version 330 core

layout (location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;
out vec2 uv;
void main() {
    gl_Position = vec4(attribPos, 1.0);
    uv = texCoord;
}

//fragment shader
#version 330 core

vec2 random2(vec2 p) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

float random(vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898, 78.233))) *
        43758.5453123);
}

vec3 noise(vec2 uv) {
    vec2 cell = floor(uv);
    vec2 localPosition = smoothstep(0.0, 1.0, fract(uv));
    //vec3 col = vec3(random(cell));

    float xCol = mix(
        random(cell),
        random(cell + vec2(1, 0)),
        localPosition.x);

    float yCol = mix(
        random(cell + vec2(0, 1)),
        random(cell + vec2(1, 1)),
        localPosition.x);

    return vec3(mix(xCol, yCol, localPosition.y));
}

in vec2 uv;
out vec4 FragColor;
void main(){ 
    vec3 col = noise(uv);
    int octaves = 50;
    float frequency = 2.0;

    float amplitude = 3.0;
    float amplitudeGain = 0.5;
    float frequencyGain = 3.0;

    for (int i = 0; i < octaves; i++) {
        col += noise(uv * frequency) * amplitude;
        amplitude *= amplitudeGain;
        frequency *= frequencyGain;
        col /= 1.04;

    }
    col = mix(col, vec3(0., 0., 0.1), 1.0 - col.x);
    col = mix(col, vec3(0.0, 1.0, 0.0), length(col) / pow(3.0, 2.));

    FragColor = vec4(col, 1.0);
}





