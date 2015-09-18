#ifndef __Model__
#define __Model__

#include "Types.h"

#include <assimp/scene.h>

struct Model *Model_Load(GLchar const *model_source);
void Model_ProcessNode(struct Model *model, struct aiNode *node, struct aiScene const *scene);
struct Mesh Model_ProcessMesh(struct aiMesh *mesh, struct aiScene const *scene);

#endif