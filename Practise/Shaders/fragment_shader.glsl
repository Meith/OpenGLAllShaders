#version 440 core

in vec4 gs_color;

out vec4 fs_color;

void main()
{
	fs_color = gs_color;
}