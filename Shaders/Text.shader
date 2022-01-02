//vertex shader
#version 330 core

layout(location = 0)in vec2 vertex;
layout(location = 1) in vec2 texCoord;
out	vec2 textCoor;

uniform	mat4	proj;

void	main()
{
	gl_Position =  vec4(vertex, 0.0, 1.0);
	textCoor = texCoord;
}

//fragment shader
#version 330 core

in	vec2 textCoor;
out vec4 color;

uniform sampler2D	textsampler;
uniform	vec3		textcolor;

void	main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textsampler, textCoor).r);
	color = vec4(textcolor, 1.0) * sampled;
}

