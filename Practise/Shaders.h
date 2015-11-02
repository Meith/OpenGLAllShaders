#ifndef __Shaders__
#define __Shaders__

#include "Types.h"

void Shaders_Init();
GLuint Shaders_CreateShaderProgram(struct ShaderPair *shader_pair, GLint num_shaders);
GLuint Shaders_CreateShader(GLchar const *shader_source, GLenum shader_type);
void Shaders_UseProgram(GLuint id);
void Shaders_UnuseProgram();
void Shaders_Destroy();

#endif