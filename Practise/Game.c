#include "Game.h"
#include "InputHandler.h"
#include "Shaders.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"

#define MAX_SHADERS 1
#define MAX_TRANSFORMS 1
#define MAX_CAMERAS 1
#define MAX_MODELS 1

void Game_Init()
{
	InputHandler_Init();

	Shaders_Init(MAX_SHADERS);
	Transform_Init(MAX_TRANSFORMS);

	Camera_Init(MAX_CAMERAS);
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

	Shaders_Destroy();
	Transform_Destroy();
	Camera_Destroy();
	
}