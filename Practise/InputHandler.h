#ifndef __InputHandler__
#define __InputHandler__

#include "Types.h"

#include <stdlib.h>

extern struct Input *input;

void InputHandler_Init();
GLuint InputHandler_Update();
void InputHandler_OnKeyPress();
void InputHandler_OnKeyRelease();
GLuint InputHandler_IsKeyPressed(SDL_Scancode key);
void InputHandler_Destroy();

#endif