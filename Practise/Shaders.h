#ifndef __Shaders__
#define __Shaders__

#include "Types.h"

struct ShaderPair *shader_pair_list = NULL;

GLuint Shaders_CreateShaderProgram(struct ShaderPair *shader_pair, GLint num_shaders);
GLuint Shaders_CreateShader(GLchar const *shader_source, GLenum shader_type);

#endif