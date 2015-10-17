#include "Transform.h"

void Transform_GetModelMatrix(struct Transform *transform, mat4x4 model_matrix)
{
	mat4x4 trans_matrix = {
		1.0f, 0.0f, 0.0f, transform->trans[0],
		0.0f, 1.0f, 0.0f, transform->trans[1],
		0.0f, 0.0f, 1.0f, transform->trans[2],
		0.0f, 0.0f, 0.0f, 1.0f
	};

	vec4_norm(transform->rot, transform->rot);
	GLfloat qx = transform->rot[0]; GLfloat qy = transform->rot[1]; GLfloat qz = transform->rot[2]; GLfloat qw = transform->rot[3];
	mat4x4 rot_matrix = {
		1.0f - 2.0f * (qy * qy + qz * qz), 2.0f * (qx * qy + qz * qw), 2.0f * (qx * qz - qy * qw), 0.0f,
		2.0f * (qx * qy - qz * qw), 1.0f - 2.0f * (qx * qx + qz * qz), 2.0f * (qy * qz + qx * qw), 0.0f,
		2.0f * (qx * qz + qy * qw), 2.0f * (qy * qz - qx * qw), 1.0f - 2.0f * (qx * qx + qy * qy), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4x4 scale_matrix = {
		transform->scale[0], 0.0f, 0.0f, 0.0f,
		0.0f, transform->scale[1], 0.0f, 0.0f,
		0.0f, 0.0f, transform->scale[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4x4 temp_mat;
	mat4x4_mul(temp_mat, trans_matrix, rot_matrix);

	mat4x4_mul(model_matrix, temp_mat, scale_matrix);
}
