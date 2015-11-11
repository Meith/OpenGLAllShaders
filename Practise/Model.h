#ifndef __Model__
#define __Model__

#include "Types.h"

#include <assimp/scene.h>

void Model_Init(GLuint num_mods);
GLuint Model_Add(GLchar const *model_source);
void Model_ProcessNode(struct Model *model, struct aiNode *node, struct aiScene const *scene);
void Model_ProcessMesh(struct Model *model, struct aiMesh *ai_mesh, struct aiScene const *scene);
void Model_LoadMaterialTextures(struct Model *model, struct Texture *textures, GLuint start_index, GLuint end_index, struct aiMaterial *material, enum aiTextureType type, GLchar const *type_name);
GLint Model_TextureFromFile(const GLchar* path, GLchar const *directory, GLboolean gamma);
void Model_Render(GLuint id, GLuint shader_program);
void Model_Destroy();

#endif