#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 tes_normal[];
in vec2 tes_texcoords[];
in vec3 tes_tangent[];
in vec3 tes_bitangent[];

out vec2 gs_texcoords;

void main()
{
	int i;
	for(i = 0; i < gl_in.length(); ++i)
	{
		gs_texcoords = tes_texcoords[i];
		gl_Position = gl_in[i].gl_Position + 5.0f;
		EmitVertex();
	}

	EndPrimitive();

	for(i = 0; i < gl_in.length(); ++i)
	{
		gs_texcoords = tes_texcoords[i];
		gl_Position = gl_in[i].gl_Position + 1.0f;
		EmitVertex();
	}

	EndPrimitive();
}