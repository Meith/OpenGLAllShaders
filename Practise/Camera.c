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

	vec3_crossprod(y, z, x);

	GLuint i;
	for (i = 0; i < 3; ++i)
	{
		view_matrix[0][i] = x[i]; x[i] *= -1.0f;
		view_matrix[1][i] = y[i]; y[i] *= -1.0f;
		view_matrix[2][i] = z[i]; z[i] *= -1.0f;
		view_matrix[3][i] = 0.0f;
	}

	view_matrix[0][3] = vec3_dotprod(x, camera->eye);
	view_matrix[1][3] = vec3_dotprod(y, camera->eye);
	view_matrix[2][3] = vec3_dotprod(z, camera->eye);
	view_matrix[3][3] = 1.0f;
}

void Camera_CreatePerspectiveMatrix(GLfloat fanf[4], GLfloat perspective_matrix[4][4])
{
	GLfloat const a = 1.0f / (GLfloat)tan(fanf[0] / 2.0f);

	perspective_matrix[0][0] = a / fanf[1];
	perspective_matrix[0][1] = 0.0f;
	perspective_matrix[0][2] = 0.0f;
	perspective_matrix[0][3] = 0.0f;

	perspective_matrix[1][0] = 0.0f;
	perspective_matrix[1][1] = a;
	perspective_matrix[1][2] = 0.0f;
	perspective_matrix[1][3] = 0.0f;

	perspective_matrix[2][0] = 0.0f;
	perspective_matrix[2][1] = 0.0f;
	perspective_matrix[2][2] = -((fanf[3] + fanf[2]) / (fanf[3] - fanf[2]));
	perspective_matrix[2][3] = -((2.0f * fanf[3] * fanf[2]) / (fanf[3] - fanf[2]));

	perspective_matrix[3][0] = 0.0f;
	perspective_matrix[3][1] = 0.0f;
	perspective_matrix[3][2] = -1.0f;
	perspective_matrix[3][3] = 0.0f;
}