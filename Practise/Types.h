#ifndef __Types__
#define __Types__

#include "Vector.h"

#define NUM_AXES 4

struct Vertex
{
	GLfloat position[NUM_AXES];
	GLfloat color[NUM_AXES];
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