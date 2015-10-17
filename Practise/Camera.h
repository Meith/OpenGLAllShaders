#ifndef __Camera__
#define __Camera__

#include "Types.h"

void Camera_CreateViewMatrix(struct Camera *camera, mat4x4 view_matrix);
void Camera_CreatePerspectiveMatrix(vec4 fanf, mat4x4 perspective_matrix);

#endif