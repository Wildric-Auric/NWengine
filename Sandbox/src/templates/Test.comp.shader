#pragma compute
#version 430 core
#pragma def LOCAL_SIZE_X LocalSizeX
#pragma def LOCAL_SIZE_Y LocalSizeY
#pragma def LOCAL_SIZE_Z LocalSizeZ

layout(local_size_x = LOCAL_SIZE_X, local_size_y = LOCAL_SIZE_X, local_size_z = LOCAL_SIZE_Z) in;

layout(rgba16f, binding = 0) uniform image2D imgOutput;
layout(std430, binding = 1) buffer data { float values[]; };

void main() {
	int idx			 = int(gl_GlobalInvocationID.y * 16 + gl_GlobalInvocationID.x);
	values[idx]		 = float(idx);
	vec4  value		 = vec4(0.0, 0.0, 0.0, 1.0);
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	value.xy		 = vec2(texelCoord.x, texelCoord.y) / 16;
	imageStore(imgOutput, texelCoord, value);
}
