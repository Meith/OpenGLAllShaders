#ifndef __InputHandler__
#define __InputHandler__

#include "Types.h"

#include <stdlib.h>

extern struct Input *input = NULL;

GLuint InputHandler_Update();
void InputHandler_OnKeyPress();
void InputHandler_OnKeyRelease();
GLuint InputHandler_IsKeyPressed(SDL_Scancode key);

#endif