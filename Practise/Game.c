#include "Game.h"
#include "InputHandler.h"

#include <stdlib.h>
#include <stdio.h>

void Game_Init()
{
	InputHandler_Init();

	__declspec(align(16)) mat4x4 mat_a = {
		4.0f, 3.0f, 2.0f, 1.0f,
		8.0f, 9.0f, 7.0f, 5.0f,
		6.0f, 3.0f, 7.0f, 1.0f,
		3.0f, 2.0f, 9.0f, 4.0f 
	};

	__declspec(align(16)) mat4x4 mat_b = {
		9.0f, 1.0f, 4.0f, 7.0f,
		2.0f, 5.0f, 6.0f, 8.0f,
		3.0f, 1.0f, 9.0f, 4.0f,
		2.0f, 5.0f, 7.0f, 8.0f
	};

	__declspec(align(16)) mat4x4 mat_r;
	__declspec(align(16)) mat4x4 mat_r_sse;

	__declspec(align(16)) vec4 vec_a = {
		1.0f, 2.0f, 3.0f, 4.0f
	};

	__declspec(align(16)) vec4 vec_b = {
		5.0f, 6.0f, 7.0f, 8.0f
	};

	__declspec(align(16)) bivec4 bivec_r;
	__declspec(align(16)) bivec4 bivec_r_sse;

	GLuint i;
	
	for (i = 0; i < 100000; ++i)
		mat4x4_mul(mat_r, mat_a, mat_b);

	for (i = 0; i < 100000; ++i)
		mat4x4_mul_sse(mat_r_sse, mat_a, mat_b);

	for (i = 0; i < 16; i += 4)
		printf("%f %f %f %f\n", mat_r[i], mat_r[i + 1], mat_r[i + 2], mat_r[i + 3]);

	printf("\n");

	for (i = 0; i < 16; i += 4)
		printf("%f %f %f %f\n", mat_r_sse[i], mat_r_sse[i + 1], mat_r_sse[i + 2], mat_r_sse[i + 3]);

	for (i = 0; i < 100000; ++i)
		vec4_wedgeprod(bivec_r, vec_a, vec_b);

	for (i = 0; i < 100000; ++i)
		vec4_wedgeprod_sse(bivec_r_sse, vec_a, vec_b);

	for (i = 0; i < 6; ++i)
		printf("%f ", bivec_r[i]);

	printf("\n");

	for (i = 0; i < 6; ++i)
		printf("%f ", bivec_r_sse[i]);
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