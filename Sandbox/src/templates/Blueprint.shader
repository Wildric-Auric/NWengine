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
uniform vec2	  uCell		   = vec2(50, 50);
uniform vec2	  uThresh	   = vec2(1, 1);
uniform int		  uDisableGrid = 0;
uniform int		  uBlackBg	   = 0;
uniform int		  uWhiteBg	   = 0;
in vec2			  uv;

out vec4 FragColor;

void main() {
	vec4 col = vec4(vec3(1.0, 1.0, 1.0) * float(uBlackBg == 0), 1.0);
	if(uBlackBg == 0 && uWhiteBg == 0)
		col = vec4(vec3(0.0, 0.0, 1.0), 1.0);
	vec2 coord		= (uv - 0.5) * uResolution;
	vec2 orthocoord = (uv - 0.5);
	orthocoord.y *= (uResolution.y / uResolution.x);
	vec2 div = mod(coord, uCell);
	bool c0	 = div.x < uThresh.x || div.x > uCell.x - uThresh.x || div.y < uThresh.y || div.y > uCell.y - uThresh.y;
	bool c1	 = abs(coord.x) < uThresh.x * 2.0 || abs(coord.y) < uThresh.y * 2.0;
	if(c0 && uDisableGrid == 0)
		col = vec4(1.0, 1.0, 1.0, 0.5);
	if(c1 && uDisableGrid == 0)
		col = vec4(0.1, 1.0, 0.0, 0.8);
	FragColor = col;
}
