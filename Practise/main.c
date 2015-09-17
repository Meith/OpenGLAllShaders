/** 
	Simple OpenGL Hello Triangle program using all OpenGL Shaders. 
	The program shows how each programmable part of the pipeline is used to modify data passed to the GPU.
	The modification is split into two stages: Compute and Render.
	Compute stage consists of the compute shader which has nothing to do with the rendering pipeline. This shader is only used to do caulations on the data sent to the GPU. 
	Render stage consists of the vertex shader, tessellation control shader, tessellation evaluation shader, geometry shader and fragment shader.
	Use of each othe above is explained in the respective shader file.
*/

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define WIDTH 1024
#define HEIGHT 768

#define NUM_AXES 4
#define NUM_VERTICES 3
#define NUM_INDICES 3

GLuint return_shader(GLchar const *shader_source, GLenum shader_type);

struct vertex
{
	GLfloat position[NUM_AXES];
	GLfloat color[NUM_AXES];
};

int main(int argc, char *argv[])
{
	/*
		Setting up opengl context with sdl.
	*/

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

	/*
		Creating compute program consisting of the compute shader.	
	*/

	GLuint comp_shader = return_shader("Shaders/compute_shader.glsl", GL_COMPUTE_SHADER);

	GLuint compute_prog = glCreateProgram();
	glAttachShader(compute_prog, comp_shader);
	glLinkProgram(compute_prog);

	GLint status;
	glGetProgramiv(compute_prog, GL_LINK_STATUS, &status);
	printf("Compute Shader link status %d\n", status);

	GLchar error_log[512];
	glGetProgramInfoLog(compute_prog, 512, NULL, error_log);
	printf("Compute Program error log: %s\n", error_log);

	glDeleteShader(comp_shader);

	/*
		Creating the render program consisting of the vertex, fragment, tessellation control, tessellation evaluation, geometry and fragment shader.
	*/

	GLuint vert_shader = return_shader("Shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	GLuint tess_cont_shader = return_shader("Shaders/tessellation_control_shader.glsl", GL_TESS_CONTROL_SHADER);
	GLuint tess_eval_shader = return_shader("Shaders/tessellation_evaluation_shader.glsl", GL_TESS_EVALUATION_SHADER);
	GLuint geom_shader = return_shader("Shaders/geometry_shader.glsl", GL_GEOMETRY_SHADER);
	GLuint frag_shader = return_shader("Shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	
	GLuint render_prog = glCreateProgram();
	glAttachShader(render_prog, vert_shader);
	glAttachShader(render_prog, tess_cont_shader);
	glAttachShader(render_prog, tess_eval_shader);
	glAttachShader(render_prog, geom_shader);
	glAttachShader(render_prog, frag_shader);
	glLinkProgram(render_prog);

	glGetProgramiv(render_prog, GL_LINK_STATUS, &status);
	printf("Render Shader link status %d\n", status);

	glGetProgramInfoLog(render_prog, 512, NULL, error_log);
	printf("Render Program error log: %s\n", error_log);

	glDeleteShader(vert_shader);
	glDeleteShader(tess_cont_shader);
	glDeleteShader(tess_eval_shader);
	glDeleteShader(geom_shader);
	glDeleteShader(frag_shader);

	/*
		Creating and setting triangle vertex data.
	*/

	struct vertex *tri_vertices = (struct vertex *)malloc(NUM_VERTICES * sizeof(struct vertex));

	tri_vertices[0].position[0] = -1.0f; tri_vertices[0].position[1] = -0.5f; tri_vertices[0].position[2] = +0.0f; tri_vertices[0].position[3] = +1.0f;
	tri_vertices[1].position[0] = +0.0f; tri_vertices[1].position[1] = -0.5f; tri_vertices[1].position[2] = +0.0f; tri_vertices[1].position[3] = +1.0f;
	tri_vertices[2].position[0] = -0.5f; tri_vertices[2].position[1] = +0.5f; tri_vertices[2].position[2] = +0.0f; tri_vertices[2].position[3] = +1.0f;

	tri_vertices[0].color[0] = +1.0f; tri_vertices[0].color[1] = +0.0f; tri_vertices[0].color[2] = +0.0f; tri_vertices[0].color[3] = +1.0f;
	tri_vertices[1].color[0] = +0.0f; tri_vertices[1].color[1] = +1.0f; tri_vertices[1].color[2] = +0.0f; tri_vertices[1].color[3] = +1.0f;
	tri_vertices[2].color[0] = +0.0f; tri_vertices[2].color[1] = +0.0f; tri_vertices[2].color[2] = +1.0f; tri_vertices[2].color[3] = +1.0f;

	/*
		Creating triangle index data.
	*/

	GLushort *tri_indices = (GLushort *)malloc(NUM_INDICES * sizeof(GLushort));
	tri_indices[0] = 0; tri_indices[1] = 1; tri_indices[2] = 2;
	
	/*
		Creating storage buffers for vertex data. Notice the type of buffer used is GL_SHADER_STORAGE_BUFFER which can be accessed not only to read but also write.
		Vertex data from the buffer will be read by the compute shader program, apply modifications, and then write back the new values into it.
	*/

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	{
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_VERTICES * sizeof(struct vertex), (struct vertex *)tri_vertices, GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	/*
		Free vertex data from the CPU after passing it to the GPU.
	*/

	free(tri_vertices);

	/*
		Creating index buffer object for triangle drawing.
	*/

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES * sizeof(GLushort), (GLushort *)tri_indices, GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*
		Free index data from the CPU after passing it to the GPU.
	*/
	free(tri_indices);


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			GLint vertex_loc = glGetAttribLocation(render_prog, "vertex");
			glVertexAttribPointer(vertex_loc, NUM_AXES, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (GLvoid *)offsetof(struct vertex, position));
			glEnableVertexAttribArray(vertex_loc);

			GLint color_loc = glGetAttribLocation(render_prog, "color");
			glVertexAttribPointer(color_loc, NUM_AXES, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (GLvoid *)offsetof(struct vertex, color));
			glEnableVertexAttribArray(color_loc);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}
	glBindVertexArray(0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLint time_loc = glGetUniformLocation(compute_prog, "time");

	GLint tessinner_loc = glGetUniformLocation(render_prog, "tess_inner");
	GLint tessouter_loc = glGetUniformLocation(render_prog, "tess_outer");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float time;

	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				break;

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(compute_prog);
		{
			time = SDL_GetTicks() / 1000.0f;
			glUniform1f(time_loc, time);
			/*
				Commands to "start" the compute shader. glDispatchCompute sets up the number of processing units in x, y, and z dimensions.
				For heavier computations, increase work/processing units.
			*/

			glDispatchCompute(3, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
		glUseProgram(0);

		glUseProgram(render_prog);
		{
			glUniform1f(tessinner_loc, 15);
			glUniform1f(tessouter_loc, 15);

			glBindVertexArray(vao);
			{
				/*
					Notice use of GL_PATCHES in draw mode, that is used because tessellation shader converts triangles into patches.
				*/

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
	glDeleteProgram(render_prog);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(gl_window);

	return 0;
}

GLuint return_shader(GLchar const *shader_source, GLenum shader_type)
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