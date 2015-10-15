#include "Types.h"
#include "Window.h"

int main(int argc, char *argv[])
{
	struct Window window;
	Window_CreateOpenGLContext(&window, "OpenGL", 1024, 768, SDL_WINDOW_OPENGL);
	Window_SetOpenGLFlags();

	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				break;
	
		Window_ClearBuffers();

		Window_SwapBuffers(&window);
	}

	Window_Destroy(&window);

	return 0;
}