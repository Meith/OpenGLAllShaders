#ifndef __Camera__
#define __Camera__

#include "Types.h"

void Camera_CreateViewMatrix(struct Camera *camera, GLfloat view_matrix[4][4]);
void Camera_CreatePerspectiveMatrix(GLfloat fanf[4], GLfloat perspective_matrix[4][4]);

#endif