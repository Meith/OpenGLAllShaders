#version 440 core

in vec4 vertex;
in vec4 color;

out vec4 vs_color;

uniform mat4 pv;

void main()
{
	gl_Position = pv * vertex;
	vs_color = color;
}