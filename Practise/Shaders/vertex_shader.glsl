#version 440 core

in vec3 vertex;

uniform mat4 view_mat;
uniform mat4 persp_mat;

void main()
{
	gl_Position = persp_mat * view_mat * vec4(vertex, 1.0);
}