#version 440 core

in vec4 vertex;
in vec2 tex_coords;

out vec2 vs_tex_coords;

void main()
{
	gl_Position = vertex;
	vs_tex_coords = tex_coords;
}