#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "Types.h"
#include "Shaders.h"

#define WIDTH 1024
#define HEIGHT 768

#define NUM_VERTICES 3
#define NUM_INDICES 3

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_Window *gl_window;
	SDL_GLContext gl_context;

	gl_window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	gl_context = SDL_GL_CreateContext(gl_window);

	glewExperimental = GL_TRUE;
	glewInit();

	printf("Using opengl version %s.\n", glGetString(GL_VERSION));
	printf("Using glsl version %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	struct ShaderPair compute_pair = { .shader_source = "Shaders/compute_shader.glsl", .shader_type = GL_COMPUTE_SHADER };
	GLuint compute_program = Shaders_CreateShaderProgram(&compute_pair, 1);

	struct ShaderPair render_pairs[5] = { [0].shader_source = "Shaders/vertex_shader.glsl", [0].shader_type = GL_VERTEX_SHADER,
		[1].shader_source = "Shaders/tessellation_control_shader.glsl", [1].shader_type = GL_TESS_CONTROL_SHADER,
		[2].shader_source = "Shaders/tessellation_evaluation_shader.glsl", [2].shader_type = GL_TESS_EVALUATION_SHADER,
		[3].shader_source = "Shaders/geometry_shader.glsl", [3].shader_type = GL_GEOMETRY_SHADER,
		[4].shader_source = "Shaders/fragment_shader.glsl", [4].shader_type = GL_FRAGMENT_SHADER };
	GLuint render_program = Shaders_CreateShaderProgram(&render_pairs[0], 5);

	struct Vertex *tri_vertices = (struct Vertex *)malloc(NUM_VERTICES * sizeof(struct Vertex));

	tri_vertices[0].position[0] = -1.0f; tri_vertices[0].position[1] = -0.5f; tri_vertices[0].position[2] = +0.0f; tri_vertices[0].position[3] = +1.0f;
	tri_vertices[1].position[0] = +0.0f; tri_vertices[1].position[1] = -0.5f; tri_vertices[1].position[2] = +0.0f; tri_vertices[1].position[3] = +1.0f;
	tri_vertices[2].position[0] = -0.5f; tri_vertices[2].position[1] = +0.5f; tri_vertices[2].position[2] = +0.0f; tri_vertices[2].position[3] = +1.0f;

	tri_vertices[0].color[0] = +1.0f; tri_vertices[0].color[1] = +0.0f; tri_vertices[0].color[2] = +0.0f; tri_vertices[0].color[3] = +1.0f;
	tri_vertices[1].color[0] = +0.0f; tri_vertices[1].color[1] = +1.0f; tri_vertices[1].color[2] = +0.0f; tri_vertices[1].color[3] = +1.0f;
	tri_vertices[2].color[0] = +0.0f; tri_vertices[2].color[1] = +0.0f; tri_vertices[2].color[2] = +1.0f; tri_vertices[2].color[3] = +1.0f;

	GLushort *tri_indices = (GLushort *)malloc(NUM_INDICES * sizeof(GLushort));
	tri_indices[0] = 0; tri_indices[1] = 1; tri_indices[2] = 2;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	{
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_VERTICES * sizeof(struct Vertex), (struct Vertex *)tri_vertices, GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	free(tri_vertices);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES * sizeof(GLushort), (GLushort *)tri_indices, GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	free(tri_indices);


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			GLint vertex_loc = glGetAttribLocation(render_program, "vertex");
			glVertexAttribPointer(vertex_loc, NUM_AXES, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, position));
			glEnableVertexAttribArray(vertex_loc);

			GLint color_loc = glGetAttribLocation(render_program, "color");
			glVertexAttribPointer(color_loc, NUM_AXES, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, color));
			glEnableVertexAttribArray(color_loc);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}
	glBindVertexArray(0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLint time_loc = glGetUniformLocation(compute_program, "time");

	GLint tessinner_loc = glGetUniformLocation(render_program, "tess_inner");
	GLint tessouter_loc = glGetUniformLocation(render_program, "tess_outer");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat time;

	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				break;

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(compute_program);
		{
			time = SDL_GetTicks() / 1000.0f;
			glUniform1f(time_loc, time);
			glDispatchCompute(3, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
		glUseProgram(0);

		glUseProgram(render_program);
		{
			glUniform1f(tessinner_loc, 15);
			glUniform1f(tessouter_loc, 15);

			glBindVertexArray(vao);
			{
				glDrawElements(GL_PATCHES, NUM_INDICES, GL_UNSIGNED_SHORT, 0);
			}
			glBindVertexArray(0);
		}
		glUseProgram(0);

		SDL_GL_SwapWindow(gl_window);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(render_program);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(gl_window);

	return 0;
}