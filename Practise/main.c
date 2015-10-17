#include "Window.h"
#include "Game.h"

int main(int argc, char *argv[])
{
	struct Window window;
	window.width = 1024;
	window.height = 768;
	window.flags = SDL_WINDOW_OPENGL;
	Window_CreateOpenGLContext(&window, "OpenGL");
	Window_SetOpenGLFlags();

	Game_Init();

	GLfloat const dt = 0.01f;
	GLfloat t = 0.0f;
	GLfloat accumulator = 0.0f;
	GLfloat previous_time = 0.0f;

	while (Game_HandleInput())
	{
		GLfloat new_time = SDL_GetTicks() / 1000.0f;
		GLfloat delta_time = new_time - previous_time;

		if (delta_time <= 0.0f)
			continue;

		previous_time = new_time;

		accumulator += delta_time;
		while (accumulator >= dt)
		{
			Game_FixedUpdate(t, dt);
			accumulator -= dt;
			t += dt;
		}

		Game_Update();
		Game_LateUpdate();

		Window_ClearBuffers();
		{
			Game_Render();
		}
		Window_SwapBuffers(&window);
	}

	Game_Destroy();
	Window_Destroy(&window);

	return 0;
}