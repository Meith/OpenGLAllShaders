#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec2 tes_tex_coords[];

out vec2 gs_tex_coords; 

uniform mat4 pv;

void main()
{
	int i;
	for(i = 0; i < gl_in.length(); ++i)
	{
		gs_tex_coords = tes_tex_coords[i];
		gl_Position = pv * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();

	for(i = 0; i < gl_in.length(); ++i)
	{
		gs_tex_coords = tes_tex_coords[i];
		gl_Position = pv * gl_in[i].gl_Position + 1.0;
		EmitVertex();
	}

	EndPrimitive();
}