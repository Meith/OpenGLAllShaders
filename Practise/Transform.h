#ifndef __Transform__
#define __Transform__

#include "Types.h"

void Transform_Init(GLuint max_trans);
GLuint Transform_Add(vec3 pos, vec3 rot, vec3 scale);
void Transform_Update();
void Transform_GetModelMatrix(GLuint id);
void Transform_Destroy();

#endif
