#include "Shaders.h"

#include <stdio.h>
#include <stdlib.h>

static GLuint *shaders_prog_list = NULL;
static GLuint num_shader_progs;

void Shaders_Init(GLuint max_shaders)
{
	shaders_prog_list = (GLuint *)malloc(max_shaders * sizeof(GLuint));
	num_shader_progs = 0;
}

GLuint Shaders_CreateShaderProgram(struct ShaderPair *shader_pair, GLint num_shaders)
{
	GLuint *shaders = (GLuint *)malloc(num_shaders * sizeof(GLuint));
	int i;
	for (i = 0; i < num_shaders; ++i)
		shaders[i] = Shaders_CreateShader(shader_pair[i].shader_source, shader_pair[i].shader_type);

	GLuint shader_program = glCreateProgram();
	for (i = 0; i < num_shaders; ++i)
		glAttachShader(shader_program, shaders[i]);
	glLinkProgram(shader_program);

	GLint status;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	printf("Shader link status %d\n", status);

	GLchar error_log[512];
	glGetProgramInfoLog(shader_program, 512, NULL, error_log);
	printf("Program error log: %s\n", error_log);

	for (i = 0; i < num_shaders; ++i)
		glDeleteShader(shaders[i]);
	free(shaders);

	shaders_prog_list[num_shader_progs] = shader_program;

	return num_shader_progs++;
}

GLuint Shaders_CreateShader(GLchar const *shader_source, GLenum shader_type)
{
	FILE *fptr;
	GLuint length;
	GLchar *buffer;

	fptr = fopen(shader_source, "rb");

	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);

	buffer = (GLchar *)malloc((length + 1) * sizeof(GLchar));
	fseek(fptr, 0, SEEK_SET);
	fread(buffer, length, sizeof(GLchar), fptr);
	buffer[length] = 0;

	fclose(fptr);

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &buffer, NULL);
	glCompileShader(shader);

	free(buffer);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	printf("Shader compile status %d\n", status);

	GLchar error_log[512];
	glGetShaderInfoLog(shader, 512, NULL, error_log);
	printf("Shader error log: %s\n", error_log);

	return shader;
}

GLuint Shaders_GetProgram(GLuint id)
{
	return shaders_prog_list[id];
}

void Shaders_UseProgram(GLuint id)
{
	glUseProgram(shaders_prog_list[id]);
}

void Shaders_UnuseProgram()
{
	glUseProgram(0);
}

void Shaders_Destroy()
{
	free(shaders_prog_list);
}