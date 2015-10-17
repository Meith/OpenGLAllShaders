#include "InputHandler.h"

void InputHandler_Init()
{
	input->keyboard_state = SDL_GetKeyboardState(0);
	input->mouse_button_state[0] = input->mouse_button_state[1] = input->mouse_button_state[2] = 0;
}

GLuint InputHandler_Update()
{
	while (SDL_PollEvent(&input->sdl_event))
	{
		switch (input->sdl_event.type)
		{
			case SDL_QUIT:
				return 0;
				break;

			case SDL_JOYAXISMOTION:
				//onJoystickAxisMove(event);
				break;

			case SDL_JOYBUTTONDOWN:
				//onJoystickButtonDown(event);
				break;

			case SDL_JOYBUTTONUP:
				//onJoystickButtonUp(event);
				break;

			case SDL_MOUSEMOTION:
				//onMouseMove(sdl_event);
				break;

			case SDL_MOUSEBUTTONDOWN:
				//onMouseButtonDown(sdl_event);
				break;

			case SDL_MOUSEBUTTONUP:
				//onMouseButtonUp(sdl_event);
				break;

			case SDL_KEYDOWN:
				InputHandler_OnKeyPress();
				break;

			case SDL_KEYUP:
				InputHandler_OnKeyRelease();
				break;

			default:
				break;
		}
	}

	return 1;
}

void InputHandler_OnKeyPress()
{
	input->keyboard_state = SDL_GetKeyboardState(0);
}

void InputHandler_OnKeyRelease()
{
	input->keyboard_state = SDL_GetKeyboardState(0);
}

GLuint InputHandler_IsKeyPressed(SDL_Scancode key)
{
	return input->keyboard_state[key];
}