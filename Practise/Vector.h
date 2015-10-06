#ifndef __Vector__
#define __Vector__

#include <GL/glew.h>

#include <math.h>

// operation within vector
#define vec_in_operation(r, a, n, o) { GLuint i; for(i = 0; i < n; ++i) r = r o a[i]; }

// operation between two vectors
#define vec_vec_operation(r, a, b, n, o) { GLuint i; for(i = 0; i < n; ++i) r[i] = a[i] o b[i]; }

// operation between vector and scalar
#define vec_scl_operation(r, a, b, n, o) { GLuint i; for(i = 0; i < n; ++i) r[i] = a[i] o b; }

#define vec(n) \
typedef GLfloat vec##n[n]; \
static _inline GLfloat vec##n##_dotprod(vec##n const a, vec##n const b) \
{ \
	GLfloat l = 0.0f; \
	GLuint i; \
	for(i=0; i<n; ++i) \
		l += b[i]*a[i]; \
	return l; \
} \
static _inline GLfloat vec##n##_len(vec##n const a) \
{ \
	return sqrtf(vec##n##_dotprod(a, a)); \
} \
static _inline void vec##n##_norm(vec##n r, vec##n const a) \
{ \
	float b = 1.0 / vec##n##_len(a); \
	vec_scl_operation(r, a, b, n, *); \
}

vec(2)
vec(3)
vec(4)

static _inline void vec3_crossprod(vec3 r, vec3 const a, vec3 const b)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

typedef vec4 mat4x4[4];

static _inline void mat4x4_mul(mat4x4 r, mat4x4 a, mat4x4 b)
{
	GLuint i, j, k;
	GLfloat sum = 0.0f;

	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
		{
			for (k = 0; k < 4; ++k)
				sum += a[i][k] * b[k][j];

			r[i][j] = sum;
			sum = 0.0f;
		}
}

#endif