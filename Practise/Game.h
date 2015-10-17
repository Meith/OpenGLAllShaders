#ifndef __Game__
#define __Game__

#include "Types.h"

void Game_Init();
GLuint Game_HandleInput();
void Game_FixedUpdate(GLfloat t, GLfloat const dt);
void Game_Update();
void Game_LateUpdate();
void Game_Render();
void Game_Destroy();

#endif