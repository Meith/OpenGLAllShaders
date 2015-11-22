#include "Camera.h"
#include "Vector.h"

#include <stdlib.h>

static struct Camera *camera_list = NULL;
static GLuint num_cameras;

void Camera_Init(GLuint max_cams)
{
	camera_list = (struct Camera *)malloc(max_cams * sizeof(struct Camera));
	num_cameras = 0;
}

GLuint Camera_Add(vec3 eye, vec3 look, vec3 up, vec4 fanf)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
	{
		camera_list[num_cameras].eye[i] = eye[i];
		camera_list[num_cameras].look[i] = look[i];
		camera_list[num_cameras].up[i] = up[i];
	}

	Camera_CreateViewMatrix(num_cameras);
	Camera_CreatePerspectiveMatrix(num_cameras, fanf);
	return num_cameras++;
}

void Camera_Update()
{
	GLuint i;
	for (i = 0; i < num_cameras; ++i)
		Camera_CreateViewMatrix(i);
}

void Camera_CreateViewMatrix(GLuint id)
{
	vec3 x;
	vec3 y;
	vec3 z;

	z[0] = camera_list[id].look[0];
	z[1] = camera_list[id].look[1];
	z[2] = camera_list[id].look[2];

	vec3_crossprod(x, camera_list[id].up, z);

	vec3_crossprod(y, z, x);

	camera_list[id].view_matrix[0] = x[0]; camera_list[id].view_matrix[1] = x[1]; camera_list[id].view_matrix[2] = x[2];
	camera_list[id].view_matrix[4] = y[0]; camera_list[id].view_matrix[5] = y[1]; camera_list[id].view_matrix[6] = y[2];
	camera_list[id].view_matrix[8] = z[0]; camera_list[id].view_matrix[9] = z[1]; camera_list[id].view_matrix[10] = z[2];
	camera_list[id].view_matrix[12] = 0.0f; camera_list[id].view_matrix[13] = 0.0f; camera_list[id].view_matrix[14] = 0.0f;

	GLuint i;
	for (i = 0; i < 3; ++i)
	{
		x[i] *= -1.0f;
		y[i] *= -1.0f;
		z[i] *= -1.0f;
	}

	camera_list[id].view_matrix[3] = vec3_dotprod(x, camera_list[id].eye);
	camera_list[id].view_matrix[7] = vec3_dotprod(y, camera_list[id].eye);
	camera_list[id].view_matrix[11] = vec3_dotprod(z, camera_list[id].eye);
	camera_list[id].view_matrix[15] = 1.0f;
}

void Camera_CreatePerspectiveMatrix(GLuint id, GLfloat fanf[4])
{
	GLfloat const a = 1.0f / (GLfloat)tan(fanf[0] / 2.0f);

	camera_list[id].perspective_matrix[0] = a / fanf[1];
	camera_list[id].perspective_matrix[1] = 0.0f;
	camera_list[id].perspective_matrix[2] = 0.0f;
	camera_list[id].perspective_matrix[3] = 0.0f;
					  
	camera_list[id].perspective_matrix[4] = 0.0f;
	camera_list[id].perspective_matrix[5] = a;
	camera_list[id].perspective_matrix[6] = 0.0f;
	camera_list[id].perspective_matrix[7] = 0.0f;
					  
	camera_list[id].perspective_matrix[8] = 0.0f;
	camera_list[id].perspective_matrix[9] = 0.0f;
	camera_list[id].perspective_matrix[10] = -((fanf[3] + fanf[2]) / (fanf[3] - fanf[2]));
	camera_list[id].perspective_matrix[11] = -((2.0f * fanf[3] * fanf[2]) / (fanf[3] - fanf[2]));
	
	camera_list[id].perspective_matrix[12] = 0.0f;
	camera_list[id].perspective_matrix[13] = 0.0f;
	camera_list[id].perspective_matrix[14] = -1.0f;
	camera_list[id].perspective_matrix[15] = 0.0f;
}

void Camera_Render(GLuint id)
{
	glUniformMatrix4fv(0, 1, GL_TRUE, camera_list[id].perspective_matrix);
	glUniformMatrix4fv(1, 1, GL_TRUE, camera_list[id].view_matrix);
}

void Camera_Destroy()
{
	free(camera_list);
}