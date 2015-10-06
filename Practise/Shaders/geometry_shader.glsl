#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

uniform mat4 pv;

void main()
{
	int i;
	for(i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = pv * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();

	for(i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = pv * gl_in[i].gl_Position + 1.0;
		EmitVertex();
	}

	EndPrimitive();
}