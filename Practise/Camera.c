#include "Camera.h"
#include "Vector.h"

void Camera_CreateViewMatrix(struct Camera *camera, GLfloat view_matrix[4][4])
{
	vec3 x;
	vec3 y;
	vec3 z;

	vec_vec_operation(z, camera->eye, camera->target, 3, -);
	vec3_norm(z, z);

	vec3_crossprod(x, camera->up, z);
	vec3_norm(x, x);

	vec3_crossprod(z, x);
}

void Camera_CreatePerspectiveMatrix(GLfloat fanf[4], GLfloat perspective_matrix[4][4])
{

}