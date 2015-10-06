#version 440 core

in vec4 vertex;
in vec4 color;

void main()
{
	gl_Position = vertex;
}