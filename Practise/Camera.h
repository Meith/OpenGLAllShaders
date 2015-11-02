#ifndef __Camera__
#define __Camera__

#include "Types.h"

void Camera_Init(GLuint max_cams);
GLuint Camera_Add(vec3 eye, vec3 look, vec3 up, vec4 fanf);
void Camera_Update();
void Camera_CreateViewMatrix(GLuint id);
void Camera_CreatePerspectiveMatrix(GLuint id, vec4 fanf);
void Camera_Destroy();

#endif