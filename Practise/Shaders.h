#ifndef __Shaders__
#define __Shaders__

#include <GL/glew.h>

#include "Types.h"

GLuint Shaders_CreateShader(GLchar const *shader_source, GLenum shader_type);
GLuint Shaders_CreateShaderProgram(struct ShaderPair *shader_pair, GLint num_shaders);

#endif