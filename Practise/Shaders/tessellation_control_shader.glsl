#version 440 core

layout (vertices = 3) out;

in vec2 vs_tex_coords[];

out vec2 tcs_tex_coords[];


uniform float tess_inner;
uniform float tess_outer;

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
	tcs_tex_coords[gl_InvocationID] = vs_tex_coords[gl_InvocationID];
}             