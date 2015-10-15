#ifndef __Types__
#define __Types__

#include "Vector.h"

struct Vertex
{
	vec4 position;
	vec2 tex_coords;
};

struct ShaderPair
{
	GLchar const *shader_source;
	GLenum shader_type;
};

struct Transform
{
	vec3 trans;
	vec4 rot;
	vec3 scale;
};

struct Camera
{
	vec3 eye;
	vec3 target;
	vec3 up;
};

#endif