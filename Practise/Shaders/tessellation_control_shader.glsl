#version 440 core

layout (vertices = 3) out;

uniform float tess_inner;
uniform float tess_outer;

in vec3 vs_normal[];
in vec2 vs_texcoords[];
in vec3 vs_tangent[];
in vec3 vs_bitangent[];

out vec3 tcs_normal[];
out vec2 tcs_texcoords[];
out vec3 tcs_tangent[];
out vec3 tcs_bitangent[];

void main()
{		
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = tess_inner;
		gl_TessLevelOuter[0] = tess_outer;
		gl_TessLevelOuter[1] = tess_outer;
		gl_TessLevelOuter[2] = tess_outer;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_normal[gl_InvocationID] = vs_normal[gl_InvocationID];
	tcs_texcoords[gl_InvocationID] = vs_texcoords[gl_InvocationID];
	tcs_tangent[gl_InvocationID] = vs_tangent[gl_InvocationID];
	tcs_bitangent[gl_InvocationID] = vs_bitangent[gl_InvocationID];
}             