#include <GL/glew.h>
#include <SDL/SDL.h>

#include <stdio.h>

#include "Types.h"
#include "Shaders.h"
#include "Model.h"
#include "Camera.h"

#define WIDTH 1024
#define HEIGHT 768

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

	struct Model *nanosuit = Model_Load("Objects/Nanosuit/nanosuit.obj");
	Camera *camera = Camera_Init();
	
	GLuint i;
	for (i = 0; i < nanosuit->textures_loaded_count; ++i)
		printf("%s\n", nanosuit->textures_loaded[i].path.data);

	GLint time_loc = glGetUniformLocation(compute_program, "time");

	GLint tessinner_loc = glGetUniformLocation(render_program, "tess_inner");
	GLint tessouter_loc = glGetUniformLocation(render_program, "tess_outer");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

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
		}
		glUseProgram(0);

		glUseProgram(render_program);
		{
			glUniform1f(tessinner_loc, 15);
			glUniform1f(tessouter_loc, 15);

			Camera_Render(camera, render_program);
			Model_Render(nanosuit, render_program);
		}
		glUseProgram(0);

		SDL_GL_SwapWindow(gl_window);
	}
	
	glDeleteProgram(compute_program);
	glDeleteProgram(render_program);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(gl_window);

	return 0;
}