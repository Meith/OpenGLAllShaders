#include "Transform.h"

#include <stdlib.h>

static struct Transform *transform_list = NULL;
static GLuint num_transforms;

void Transform_Init(max_trans)
{
	transform_list = (struct Transform *)malloc(max_trans * sizeof(struct Transform));
	num_transforms = 0;
}

GLuint Transform_Add(vec3 pos, vec3 rot, vec3 scale)
{
	vec4 q_rot;
	vec3_quat(q_rot, rot);

	GLuint i;
	for (i = 0; i < 3; ++i)
	{
		transform_list[num_transforms].pos[i] = pos[i];
		transform_list[num_transforms].rot[i] = q_rot[i];
		transform_list[num_transforms].scale[i] = scale[i];
	}

	transform_list[num_transforms].rot[3] = q_rot[3];

	Transform_GetModelMatrix(num_transforms);

	return num_transforms++;
}

void Transform_Update()
{
	GLuint i;
	for (i = 0; i < num_transforms; ++i)
		Transform_GetModelMatrix(i);
}

void Transform_GetModelMatrix(GLuint id)
{
	mat4x4 trans_matrix = {
		1.0f, 0.0f, 0.0f, transform_list[id].pos[0],
		0.0f, 1.0f, 0.0f, transform_list[id].pos[1],
		0.0f, 0.0f, 1.0f, transform_list[id].pos[2],
		0.0f, 0.0f, 0.0f, 1.0f
	};

	GLfloat qx =  transform_list[id].rot[0]; 
	GLfloat qy =  transform_list[id].rot[1]; 
	GLfloat qz =  transform_list[id].rot[2]; 
	GLfloat qw =  transform_list[id].rot[3];
	mat4x4 rot_matrix = {
		1.0f - 2.0f * (qy * qy + qz * qz), 2.0f * (qx * qy + qz * qw), 2.0f * (qx * qz - qy * qw), 0.0f,
		2.0f * (qx * qy - qz * qw), 1.0f - 2.0f * (qx * qx + qz * qz), 2.0f * (qy * qz + qx * qw), 0.0f,
		2.0f * (qx * qz + qy * qw), 2.0f * (qy * qz - qx * qw), 1.0f - 2.0f * (qx * qx + qy * qy), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4x4 scale_matrix = {
		transform_list[id].scale[0], 0.0f, 0.0f, 0.0f,
		0.0f, transform_list[id].scale[1], 0.0f, 0.0f,
		0.0f, 0.0f, transform_list[id].scale[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4x4 temp_mat;
	mat4x4_mul(temp_mat, trans_matrix, rot_matrix);

	mat4x4_mul(transform_list[id].model_matrix, temp_mat, scale_matrix);
}

void Transform_Destroy()
{
	free(transform_list);
}
