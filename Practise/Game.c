#include "Game.h"
#include "InputHandler.h"
#include "Shaders.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"

#include <stdio.h>

void Game_Init()
{
	InputHandler_Init();
	Transform_Init();
	Camera_Init();
	Shaders_Init();
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
	Transform_Update();
}

void Game_LateUpdate()
{
	Camera_Update();
}

void Game_Render()
{

}

void Game_Destroy()
{
	InputHandler_Destroy();
	Transform_Destroy();
	Camera_Destroy();
	Shaders_Destroy();
}