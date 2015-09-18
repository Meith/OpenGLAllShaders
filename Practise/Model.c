#include "Model.h"

#include <assimp/postprocess.h>

#include <stdlib.h>
#include <string.h>

struct Model *Model_Load(GLchar const *model_source)
{
	struct Model *model = (struct Model *)malloc(sizeof(struct Model));

	struct aiScene const *scene = aiImportFile(model_source, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	strncpy(model->directory, model_source, strrchr(model_source, '/'));

	Model_ProcessNode(model, scene->mRootNode, scene);
}

void Model_ProcessNode(struct Model *model, struct aiNode *node, struct aiScene const *scene)
{
	static GLuint mesh_count = 0;
	GLuint i;
	for (i = 0; i < node->mNumMeshes; ++i)
	{
		struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes = (struct Mesh *)realloc(model->meshes, (++mesh_count) * sizeof(struct Mesh));
		model->meshes[mesh_count - 1] = Model_ProcessMesh(mesh, scene);
	}

	for (i = 0; i < node->mNumChildren; ++i)
	{
		Model_ProcessNode(model, node->mChildren[i], scene);
	}
}

struct Mesh Model_ProcessMesh(struct aiMesh *mesh, struct aiScene const *scene)
{
	GLuint vertex_count = mesh->mNumVertices;
	struct Vertex *vertices = (struct Vertex *)malloc(vertex_count * sizeof(struct Vertex));
	GLuint *indices;
	struct Texture *textures;

	GLuint i;
	for (i = 0; i < vertex_count; ++i)
	{
		vertices[i].position[0] = mesh->mVertices[i].x;
		vertices[i].position[1] = mesh->mVertices[i].y;
		vertices[i].position[2] = mesh->mVertices[i].z;

		vertices[i].normal[0] = mesh->mNormals[i].x;
		vertices[i].normal[1] = mesh->mNormals[i].y;
		vertices[i].normal[2] = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertices[i].texcoords[0] = mesh->mTextureCoords[0][i].x;
			vertices[i].texcoords[1] = mesh->mTextureCoords[0][i].y;
		}

		else
		{
			vertices[i].texcoords[0] = 0.0f;
			vertices[i].texcoords[1] = 0.0f;
		}

		vertices[i].tangent[0] = mesh->mTangents[i].x;
		vertices[i].tangent[1] = mesh->mTangents[i].y;
		vertices[i].tangent[2] = mesh->mTangents[i].z;

		vertices[i].bitangent[0] = mesh->mBitangents[i].x;
		vertices[i].bitangent[1] = mesh->mBitangents[i].y;
		vertices[i].bitangent[2] = mesh->mBitangents[i].z;
	}

	GLuint current_index_count = 0;
	GLuint total_index_count = 0;
	GLuint j;
	for (i = 0; i < mesh->mNumFaces; ++i)
	{
		struct aiFace face = mesh->mFaces[i];
		total_index_count += face.mNumIndices;
		indices = (GLuint *)realloc(indices, total_index_count * sizeof(GLuint));

		for (j = current_index_count; j < total_index_count; ++j)
			indices[j] = face.mIndices[j];
	}

	if (mesh->mMaterialIndex >= 0)
	{
		struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		GLuint texture_counts[4] = { 0 };
		texture_counts[0] = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
		texture_counts[1] = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
		texture_counts[2] = aiGetMaterialTextureCount(material, aiTextureType_HEIGHT);
		texture_counts[3] = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);

		textures = (struct Texture *)malloc((texture_counts[0] + texture_counts[1] + texture_counts[2] + texture_counts[3]) * sizeof(struct Texture));
		Model_LoadMaterialTextures(textures, 0, texture_counts[0], material, aiTextureType_DIFFUSE, "texture_diffuse");
	}
}

void Model_LoadMaterialTextures(struct Texture *textures, GLuint start_index, GLuint end_index, struct aiMaterial *material, enum aiTextureType type, GLchar const *typeName)
{
	GLuint i;
	for (i = start_index; i < end_index; ++i)
	{
		struct aiString string;
		aiGetMaterialTexture(material, type, i, &string, NULL, NULL, NULL, NULL, NULL, NULL);

		GLboolean skip = 0;
	}
}