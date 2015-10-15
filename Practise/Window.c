#include "Window.h"

#include <SDL/SDL.h>

#include <stdio.h>

void Window_CreateOpenGLContext(struct Window *window, const GLchar *name, GLuint width, GLuint height, GLenum flags)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	window->gl_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	window->gl_context = SDL_GL_CreateContext(window->gl_window);

	glewExperimental = GL_TRUE;
	glewInit();

	printf("Using opengl version %s.\n", glGetString(GL_VERSION));
	printf("Using glsl version %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Window_SetOpenGLFlags()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Window_ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window_SwapBuffers(struct Window *window)
{
	SDL_GL_SwapWindow(window->gl_window);
}

void Window_Destroy(struct Window *window)
{
	SDL_GL_DeleteContext(window->gl_context);
	SDL_DestroyWindow(window->gl_window);
}