#version 440 core

in vec4 vertex;
in vec4 color;

out vec4 vs_color;

void main()
{
	gl_Position = vertex;
	vs_color = color;
}