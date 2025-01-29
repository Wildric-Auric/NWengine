
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


vec2 curve(vec2 uv)
{
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;	
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
	uv  = (uv / 2.0) + 0.5;
	uv =  uv *0.92 + 0.04;
	return uv;
}

#define iChannel0 uTex0
void main()
{
    float iTime = uTime;
   

    vec2 q = uv;
    vec2 uv0 = curve( uv );

    vec3 oricol = texture( iChannel0, vec2(q.x,q.y) ).xyz;
    vec3 col;
	float x =  sin(0.3*iTime+uv.y*21.0)*sin(0.7*iTime+uv.y*29.0)*sin(0.3+0.33*iTime+uv.y*31.0)*0.0017;

    col.r = texture(iChannel0,vec2(x+uv0.x+0.001,uv0.y+0.001)).x+0.05;
    col.g = texture(iChannel0,vec2(x+uv0.x+0.000,uv0.y-0.002)).y+0.05;
    col.b = texture(iChannel0,vec2(x+uv0.x-0.002,uv0.y+0.000)).z+0.05;
    col.r += 0.08*texture(iChannel0,0.75*vec2(x+0.025, -0.027)+vec2(uv0.x+0.001,uv0.y+0.001)).x;
    col.g += 0.05*texture(iChannel0,0.75*vec2(x+-0.022, -0.02)+vec2(uv0.x+0.000,uv0.y-0.002)).y;
    col.b += 0.08*texture(iChannel0,0.75*vec2(x+-0.02, -0.018)+vec2(uv0.x-0.002,uv0.y+0.000)).z;

    col = clamp(col*0.6+0.4*col*col*1.0,0.0,1.0);

    float vig = (0.0 + 1.0*16.0*uv0.x*uv0.y*(1.0-uv0.x)*(1.0-uv0.y));
	col *= vec3(pow(vig,0.3));

    col *= vec3(0.95,1.05,0.95);
	col *= 2.8;

    float scans = clamp(0.35+0.35 * sin(2.5 * iTime + uv0.y * 999.0), 0.0, 1.0);
	
	float s = pow(scans,2.7);
	col = col*vec3( 0.4+0.7*s) ;

    col *= 1.0+0.01*sin(110.0*iTime);
	if (uv0.x < 0.0 || uv0.x > 1.0)
		col *= 0.0;
	if (uv0.y < 0.0 || uv0.y > 1.0)
		col *= 0.0;
	
   col = mix(col, col * col, 0.5);
	
    float comp = smoothstep( 0.1, 0.9, sin(iTime) );
 
// Remove the next line to stop cross-fade between original and postprocess
//	col = mix( col, oricol, comp );

    FragColor = vec4(col,1.0);
}
        