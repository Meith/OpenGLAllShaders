#version 450 core

in vec2 vs_texcoords;

out vec4 fs_color;

layout (location = 3) uniform sampler2D texture_diffuse1;

void main()
{
	fs_color = texture(texture_diffuse1, vs_texcoords);
}