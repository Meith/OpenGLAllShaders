#include "Game.h"
#include "InputHandler.h"

#include <stdlib.h>
#include <stdio.h>

struct Input *input = NULL;

void Game_Init()
{
	input = (struct Input *)malloc(sizeof(struct Input));
	InputHandler_Init();
}

GLuint Game_HandleInput()
{
	return InputHandler_Update();
}

void Game_FixedUpdate(GLfloat t, GLfloat const dt)
{

}

void Game_Update()
{
	if (InputHandler_IsKeyPressed(SDL_SCANCODE_A))
		printf("A\n");
}

void Game_LateUpdate()
{

}

void Game_Render()
{

}

void Game_Destroy()
{
	
}