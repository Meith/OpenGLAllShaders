#ifndef __Types__
#define __Types__

#include <GL/glew.h>

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

#endif