#ifndef __Types__
#define __Types__

#include <GL/glew.h>

struct Shader
{
	GLuint program;
	GLuint shaders[5];
	GLint num_shaders;
};

#endif