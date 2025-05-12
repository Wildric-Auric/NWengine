#pragma vertex
#version 330 core

layout(location = 0) in vec3 attribPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 uMvp = mat4(1.0);
uniform vec2 uResolution;

out vec2 uv;

void main() {
	gl_Position = uMvp * vec4(attribPos, 1.0);
	uv			= texCoord;
};

#pragma fragment
#version 330 core

uniform float	  uTime;
uniform sampler2D uTex0;
uniform vec2	  uResolution;
uniform vec2	  uCell;
in vec2			  uv;

out vec4 FragColor;

void main() {
	vec4 col		= vec4(0.0, 0.0, 1.0, 1.0);
	vec2 coord		= (uv - 0.5) * uResolution;
	vec2 orthocoord = (uv - 0.5);
	orthocoord.y *= (uResolution.y / uResolution.x);
	vec2 div   = mod(coord, uCell) / uCell;
	vec2 tresh = vec2(0.02, 0.02);
	if(div.x < tresh.x || div.x > 1.0 - tresh.x || div.y < tresh.y || div.y > 1.0 - tresh.y)
		col = vec4(1.0, 1.0, 1.0, 0.5);
	if(abs(orthocoord.x) < tresh.x * 0.1 || abs(orthocoord.y) < tresh.y * 0.1)
		col = vec4(0.1, 1.0, 0.0, 0.8);
	FragColor = col;
}
