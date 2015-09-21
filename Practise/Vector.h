#ifndef __Vector__
#define __Vector__

#include <GL/glew.h>

#include <math.h>

#define FOR(N)  GLuint i; for(i = 0; i < N; ++i)

#define VectorComponentOperation(R, A, N, O) { FOR(N) R O= A[i]; }
#define VectorScalarOperation(R, A, S, N, O) { FOR(N) R = A[i] O S; }
#define VectorOperation(R, A, B, N, O) { FOR(N) R[i] = A[i] O B[i]; }

#endif