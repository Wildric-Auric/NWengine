//vertex shader
#version 330 core

layout(location = 0)in vec4 vertex;
out	vec2 textCoor;

uniform	mat4	proj;

void	main()
{
	gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);
	textCoor = vertex.zw;
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

