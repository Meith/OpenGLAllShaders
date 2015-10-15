#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "Types.h"
#include "Shaders.h"
#include "Camera.h"

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

	struct ShaderPair mandelbort_cs_pair = { .shader_source = "Shaders/mandelbrot_cs.glsl", .shader_type = GL_COMPUTE_SHADER };
	GLuint mandelbort_cs_program = Shaders_CreateShaderProgram(&mandelbort_cs_pair, 1);

	struct ShaderPair render_pairs[5] = { [0].shader_source = "Shaders/vertex_shader.glsl", [0].shader_type = GL_VERTEX_SHADER,
		[1].shader_source = "Shaders/tessellation_control_shader.glsl", [1].shader_type = GL_TESS_CONTROL_SHADER,
		[2].shader_source = "Shaders/tessellation_evaluation_shader.glsl", [2].shader_type = GL_TESS_EVALUATION_SHADER,
		[3].shader_source = "Shaders/geometry_shader.glsl", [3].shader_type = GL_GEOMETRY_SHADER,
		[4].shader_source = "Shaders/fragment_shader.glsl", [4].shader_type = GL_FRAGMENT_SHADER };
	GLuint render_program = Shaders_CreateShaderProgram(&render_pairs[0], 5);

	struct Camera camera = { .eye = { 0.0f, 0.0f, 2.0f }, .target = { 0.0f, 0.0f, 0.0f }, .up = { 0.0f, 1.0f, 0.0f } };
	mat4x4 view_matrix;
	Camera_CreateViewMatrix(&camera, view_matrix);

	GLfloat fanf[4] = { 1.0472f, 1.333333f, 0.1f, 100.0f };
	mat4x4 perpective_matrix;
	Camera_CreatePerspectiveMatrix(fanf, perpective_matrix);

	mat4x4 pv;
	mat4x4_mul(pv, perpective_matrix, view_matrix);

	struct Vertex *tri_vertices = (struct Vertex *)malloc(NUM_VERTICES * sizeof(struct Vertex));

	tri_vertices[0].position[0] = -1.0f; tri_vertices[0].position[1] = -1.0f; tri_vertices[0].position[2] = +0.0f; tri_vertices[0].position[3] = +1.0f; tri_vertices[0].tex_coords[0] = 0.0f; tri_vertices[0].tex_coords[1] = 0.0f;
	tri_vertices[1].position[0] = +1.0f; tri_vertices[1].position[1] = -1.0f; tri_vertices[1].position[2] = +0.0f; tri_vertices[1].position[3] = +1.0f; tri_vertices[1].tex_coords[0] = 1.0f; tri_vertices[1].tex_coords[1] = 0.0f;
	tri_vertices[2].position[0] = +0.0f; tri_vertices[2].position[1] = +1.0f; tri_vertices[2].position[2] = +0.0f; tri_vertices[2].position[3] = +1.0f; tri_vertices[2].tex_coords[0] = 0.5f; tri_vertices[2].tex_coords[1] = 1.0f;

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

	GLuint tbo;
	glGenTextures(1, &tbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tbo);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 256, 256);
	glBindImageTexture(0, tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, position));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, tex_coords));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
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

	GLint pv_loc = glGetUniformLocation(render_program, "pv");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	//GLfloat time;
	GLfloat center[2] = { 0.001643721971153f, 0.822467633298876f };
	GLfloat cheight = 2.0f;
	GLfloat cwidth = cheight;
	vec4 bbox = { center[0] - cwidth / 2.0f, center[1] - cheight / 2.0f, center[0] + cwidth / 2.0f, center[1] + cheight / 2.0f };

	glUseProgram(mandelbort_cs_program);
	{
		glUniform4fv(glGetUniformLocation(mandelbort_cs_program, "CompWindow"), 1, &bbox[0]);
	}
	glUseProgram(0);

	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				break;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mandelbort_cs_program);
		{
			glDispatchCompute(256 / 32, 256 / 32, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}
		glUseProgram(0);

		/*glUseProgram(compute_program);
		{
			time = SDL_GetTicks() / 1000.0f;
			glUniform1f(time_loc, time);
			glDispatchCompute(3, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
		glUseProgram(0);*/

		glUseProgram(render_program);
		{
			glUniform1f(tessinner_loc, 15);
			glUniform1f(tessouter_loc, 15);

			glUniformMatrix4fv(pv_loc, 1, GL_TRUE, pv);

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
	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(render_program);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(gl_window);

	return 0;
}