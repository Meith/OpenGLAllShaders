#ifndef __Mesh__
#define __Mesh__

#include "Types.h"

struct Mesh Mesh_Init(struct Vertex *vertices, GLuint vertex_count, GLuint *indices, GLuint index_count, struct Texture *textures, GLuint texture_count);
void Mesh_Setup(struct Mesh *mesh);
void Mesh_Render(struct Mesh *mesh, GLuint shader_program);

#endif