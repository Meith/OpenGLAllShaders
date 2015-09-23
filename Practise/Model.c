#include "Model.h"
#include "Mesh.h"

#include <assimp/postprocess.h>
#include <SOIL/SOIL.h>

#include <stdlib.h>
#include <string.h>

struct Model *Model_Load(GLchar const *model_source)
{
	struct Model *model = (struct Model *)malloc(sizeof(struct Model));
	model->mesh_count = 0;
	model->meshes = NULL;
	model->textures_loaded = NULL;
	model->textures_loaded_count = 0;

	struct aiScene const *scene = aiImportFile(model_source, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	char *directory_length = strrchr(model_source, '/') + 1;

	GLuint i;
	for (i = 0; model_source + i != directory_length; ++i)
		model->directory[i] = model_source[i];
	model->directory[i] = 0;

	Model_ProcessNode(model, scene->mRootNode, scene);

	return model;
}

void Model_ProcessNode(struct Model *model, struct aiNode *node, struct aiScene const *scene)
{
	GLuint i;
	for (i = 0; i < node->mNumMeshes; ++i)
	{
		struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes = (struct Mesh *)realloc(model->meshes, (model->mesh_count + 1) * sizeof(struct Mesh));
		model->meshes[model->mesh_count++] = Model_ProcessMesh(model, mesh, scene);
	}

	for (i = 0; i < node->mNumChildren; ++i)
	{
		Model_ProcessNode(model, node->mChildren[i], scene);
	}
}

struct Mesh Model_ProcessMesh(struct Model *model, struct aiMesh *mesh, struct aiScene const *scene)
{
	GLuint vertex_count = mesh->mNumVertices;
	struct Vertex *vertices = (struct Vertex *)malloc(vertex_count * sizeof(struct Vertex));
	GLuint *indices = NULL;
	struct Texture *textures = NULL;

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
	for (i = 0; i < mesh->mNumFaces; ++i)
	{
		struct aiFace face = mesh->mFaces[i];
		total_index_count += face.mNumIndices;
		indices = (GLuint *)realloc(indices, total_index_count * sizeof(GLuint));
		GLuint j;
		for (j = 0; current_index_count < total_index_count; ++current_index_count, ++j)
			indices[current_index_count] = face.mIndices[j];
	}

	GLuint total_texture_count = 0;
	if (mesh->mMaterialIndex >= 0)
	{
		struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		GLuint texture_counts[4] = { 0 };
		texture_counts[0] = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
		texture_counts[1] = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
		texture_counts[2] = aiGetMaterialTextureCount(material, aiTextureType_HEIGHT);
		texture_counts[3] = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);
		total_texture_count = texture_counts[0] + texture_counts[1] + texture_counts[2] + texture_counts[3];

		textures = (struct Texture *)malloc((total_texture_count)* sizeof(struct Texture));
		Model_LoadMaterialTextures(model, textures, 0, texture_counts[0], material, aiTextureType_DIFFUSE, "texture_diffuse");
		Model_LoadMaterialTextures(model, textures, texture_counts[0], texture_counts[0] + texture_counts[1], material, aiTextureType_SPECULAR, "texture_specular");
		Model_LoadMaterialTextures(model, textures, texture_counts[0] + texture_counts[1], texture_counts[0] + texture_counts[1] + texture_counts[2], material, aiTextureType_HEIGHT, "texture_normal");
		Model_LoadMaterialTextures(model, textures, texture_counts[0] + texture_counts[1] + texture_counts[2], total_texture_count, material, aiTextureType_AMBIENT, "texture_height");
	}

	return Mesh_Init(vertices, vertex_count, indices, total_index_count, textures, total_texture_count);
}

void Model_LoadMaterialTextures(struct Model *model, struct Texture *textures, GLuint start_index, GLuint end_index, struct aiMaterial *material, enum aiTextureType type, GLchar const *type_name)
{
	GLuint i;
	GLuint j;
	GLuint k = start_index;
	GLuint texture_type_count = end_index - start_index;
	for (i = 0; i < texture_type_count; ++i)
	{
		struct aiString string;
		aiGetMaterialTexture(material, type, i, &string, NULL, NULL, NULL, NULL, NULL, NULL);

		GLboolean skip = 0;
		for (j = 0; j < model->textures_loaded_count; ++j)
		{
			if (model->textures_loaded[j].path.data == string.data)
			{
				textures[k++] = model->textures_loaded[j];
				skip = 1;
				break;
			}
		}

		if (!skip)
		{
			textures[k].id = Model_TextureFromFile(string.data, model->directory, 0);
			strcpy(textures[k].type, type_name);
			textures[k].path = string;
			model->textures_loaded = (struct Texture *)realloc(model->textures_loaded, (model->textures_loaded_count + 1) * sizeof(struct Texture));
			model->textures_loaded[model->textures_loaded_count++] = textures[k++];
		}
	}
}

GLint Model_TextureFromFile(const GLchar* path, GLchar const *directory, GLboolean gamma)
{
	GLchar filename[100];
	strcpy(filename, directory);
	strcat(filename, path);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	GLint width, height;
	unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, gamma ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture_id;
}

void Model_Render(struct Model *model, GLuint shader_prog)
{
	GLuint i;
	for (i = 0; i < model->mesh_count; ++i)
	{
		Mesh_Render(&model->meshes[i], shader_prog);
	}
}