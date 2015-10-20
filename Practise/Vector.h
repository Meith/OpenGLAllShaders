#ifndef __Vector__
#define __Vector__

#include <GL/glew.h>

#include <math.h>
#include <xmmintrin.h>

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
	for(i = 0; i < n; ++i) \
		l += a[i] * b[i]; \
	return l; \
} \
static _inline GLfloat vec##n##_len(vec##n const a) \
{ \
	return sqrtf(vec##n##_dotprod(a, a)); \
} \
static _inline void vec##n##_norm(vec##n r, vec##n const a) \
{ \
	float b = 1.0f / vec##n##_len(a); \
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

typedef GLfloat mat4x4[16];

static _inline void mat4x4_mul(mat4x4 r, mat4x4 const a, mat4x4 const b)
{
	GLuint i, j;

	for (i = 0; i < 16; i += 4)
		for (j = 0; j < 4; ++j)
			r[i + j] = a[i] * b[j] + a[i + 1] * b[j + 4] + a[i + 2] * b[j + 8] + a[i + 3] * b[j + 12];
}

static _inline void mat4x4_mul_sse(mat4x4 r, mat4x4 const a, mat4x4 const b)
{
	__m128 a_line, b_line, r_line;

	GLuint i, j;

	for (i = 0; i < 16; i += 4)
	{
		a_line = _mm_set1_ps(a[i]);
		b_line = _mm_load_ps(b);

		r_line = _mm_mul_ps(a_line, b_line);

		for (j = 1; j < 4; ++j)
		{
			a_line = _mm_set1_ps(a[i + j]);
			b_line = _mm_load_ps(&b[j * 4]);

			r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
		}

		_mm_store_ps(&r[i], r_line);
	}
}

#endif