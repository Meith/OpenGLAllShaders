#version 440 core

in vec2 gs_tex_coords;

out vec4 fs_color;

uniform sampler2D tex_sample;

void main()
{
	fs_color = texture(tex_sample, gs_tex_coords);
}