
#pragma vertex 
#version 330 core
        
layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;
        
uniform mat4 uMvp = mat4(1.0);
uniform vec2 uResolution;
        
out vec2 uv;
out vec4 screenPos;
        
void main() {
    gl_Position = uMvp * vec4(attribPos, 1.0);
    uv = texCoord;
    screenPos = gl_Position;
};
#pragma fragment 
#version 330 core
        
uniform float uTime;
uniform sampler2D uTex0;
uniform vec2 uResolution;
uniform vec2 uMouse;
in vec2 uv;
in vec4 screenPos;
        
out vec4 FragColor;

float rand(in vec2 uv) {
    return fract(sin(dot(uv,  vec2(12.9898,78.233))) * 43758.5453123);
}

float noise(in vec2 pos) {
    vec2 localPos = fract(pos);
    float a        = rand(floor(pos));
    float b        = rand(floor(pos + vec2(1.0, 0.0)));
    float c        = rand(floor(pos + vec2(1.0, 1.0)));
    float d        = rand(floor(pos + vec2(0.0, 1.0)));
    
    vec2 xy = localPos*localPos*(3.0 - 2.0*localPos);
    
    return mix(mix(a, b, xy.x), mix(d, c, xy.x), xy.y);
    
}

float fbm(in vec2 pos) {
   float ret  = 0.0;
   float freq = 1.0;
   
   ret += noise(pos);
   ret += 0.5     * noise(2.0   * pos);
   ret += 0.25    * noise(4.0   * pos);
   ret += 0.125   * noise(8.0   * pos);
   ret += 0.0625  * noise(16.0  * pos);
   ret += 0.03125 * noise(32.0  * pos);
   
   return ret;
}
        
void main() {
    float yy = uv.y * 0.3;
    float xx =  uv.x + 0.004 * sin(uTime*6.0 + yy * 400.0) * noise(500.0 * uv);

    vec4 col = vec4(  texture(uTex0, vec2(xx, ( 0.6 - yy ) ) ).xyz * 0.7, 1.0);
    FragColor = col;
};