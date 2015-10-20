#include "Game.h"
#include "InputHandler.h"

#include <stdlib.h>
#include <stdio.h>

void Game_Init()
{
	InputHandler_Init();

	mat4x4 a = { 
		4.0f, 3.0f, 2.0f, 1.0f,
		8.0f, 9.0f, 7.0f, 5.0f,
		6.0f, 3.0f, 7.0f, 1.0f,
		3.0f, 2.0f, 9.0f, 4.0f 
	};

	mat4x4 b = {
		9.0f, 1.0f, 4.0f, 7.0f,
		2.0f, 5.0f, 6.0f, 8.0f,
		3.0f, 1.0f, 9.0f, 4.0f,
		2.0f, 5.0f, 7.0f, 8.0f
	};

	mat4x4 r;
	mat4x4 r_sse;

	mat4x4_mul(r, a, b);
	mat4x4_mul_sse(r_sse, a, b);

	GLuint i;
	for (i = 0; i < 16; i += 4)
		printf("%f %f %f %f\n", r[i], r[i + 1], r[i + 2], r[i + 3]);

	printf("\n");

	for (i = 0; i < 16; i += 4)
		printf("%f %f %f %f\n", r[i], r[i + 1], r[i + 2], r[i + 3]);
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