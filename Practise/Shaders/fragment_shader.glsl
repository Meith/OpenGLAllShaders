#version 440 core

in vec2 gs_texcoords;

out vec4 fs_color;

uniform sampler2D texture_diffuse1;

void main()
{
	fs_color = texture(texture_diffuse1, gs_texcoords);
}