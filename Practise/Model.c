#include "Model.h"
#include "Mesh.h"

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <SOIL/SOIL.h>

#include <stdlib.h>
#include <string.h>

static struct Model *model_list = NULL;
static GLuint num_models;

void Model_Init(num_mods)
{
	model_list = (struct Model *)malloc(num_mods * sizeof(struct Model));
	num_models = 0;
}

GLuint Model_Add(GLchar const *model_source)
{
	model_list[num_models].meshes = NULL;
	model_list[num_models].mesh_count = 0;
	model_list[num_models].textures_loaded = NULL;
	model_list[num_models].textures_loaded_count = 0;

	struct aiScene const *scene = aiImportFile(model_source, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	char *directory_length = strrchr(model_source, '/') + 1;

	GLuint i;
	for (i = 0; model_source + i != directory_length; ++i)
		model_list[num_models].directory[i] = model_source[i];
	model_list[num_models].directory[i] = 0;

	Model_ProcessNode(&model_list[num_models], scene->mRootNode, scene);

	return num_models++;
}

void Model_ProcessNode(struct Model *model, struct aiNode *node, struct aiScene const *scene)
{
	model->meshes = (struct Mesh*)realloc(model->meshes, (model->mesh_count + node->mNumMeshes) * sizeof(struct Mesh));

	GLuint i;
	for (i = 0; i < node->mNumMeshes; ++i)
	{
		struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
		Model_ProcessMesh(model, ai_mesh, scene);
	}

	for (i = 0; i < node->mNumChildren; ++i)
	{
		Model_ProcessNode(model, node->mChildren[i], scene);
	}
}

void Model_ProcessMesh(struct Model *model, struct aiMesh *ai_mesh, struct aiScene const *scene)
{
	struct Mesh *current_mesh = &model->meshes[model->mesh_count];
	
	current_mesh->vertex_count = ai_mesh->mNumVertices;
	current_mesh->vertices = (struct Vertex *)malloc(current_mesh->vertex_count * sizeof(struct Vertex));

	GLuint i;
	for (i = 0; i < current_mesh->vertex_count; ++i)
	{
		current_mesh->vertices[i].position[0] = ai_mesh->mVertices[i].x;
		current_mesh->vertices[i].position[1] = ai_mesh->mVertices[i].y;
		current_mesh->vertices[i].position[2] = ai_mesh->mVertices[i].z;
	
		current_mesh->vertices[i].normal[0] = ai_mesh->mNormals[i].x;
		current_mesh->vertices[i].normal[1] = ai_mesh->mNormals[i].y;
		current_mesh->vertices[i].normal[2] = ai_mesh->mNormals[i].z;

		if (ai_mesh->mTextureCoords[0])
		{
			current_mesh->vertices[i].texcoords[0] = ai_mesh->mTextureCoords[0][i].x;
			current_mesh->vertices[i].texcoords[1] = ai_mesh->mTextureCoords[0][i].y;
		}

		else
		{
			current_mesh->vertices[i].texcoords[0] = 0.0f;
			current_mesh->vertices[i].texcoords[1] = 0.0f;
		}

		current_mesh->vertices[i].tangent[0] = ai_mesh->mTangents[i].x;
		current_mesh->vertices[i].tangent[1] = ai_mesh->mTangents[i].y;
		current_mesh->vertices[i].tangent[2] = ai_mesh->mTangents[i].z;

		current_mesh->vertices[i].bitangent[0] = ai_mesh->mBitangents[i].x;
		current_mesh->vertices[i].bitangent[1] = ai_mesh->mBitangents[i].y;
		current_mesh->vertices[i].bitangent[2] = ai_mesh->mBitangents[i].z;
	}

	current_mesh->indices = NULL;
	current_mesh->index_count = 0;
	GLuint current_index_count = 0;
	for (i = 0; i < ai_mesh->mNumFaces; ++i)
	{
		struct aiFace face = ai_mesh->mFaces[i];
		current_mesh->index_count += face.mNumIndices;
		current_mesh->indices = (GLuint *)realloc(current_mesh->indices, current_mesh->index_count * sizeof(GLuint));
		GLuint j;
		for (j = 0; current_index_count < current_mesh->index_count; ++current_index_count, ++j)
			current_mesh->indices[current_index_count] = face.mIndices[j];
	}

	current_mesh->textures = NULL;
	current_mesh->texture_count = 0;
	if (ai_mesh->mMaterialIndex >= 0)
	{
		struct aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];

		GLuint texture_counts[4] = { 0 };
		texture_counts[0] = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
		texture_counts[1] = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
		texture_counts[2] = aiGetMaterialTextureCount(material, aiTextureType_HEIGHT);
		texture_counts[3] = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);
		current_mesh->texture_count = texture_counts[0] + texture_counts[1] + texture_counts[2] + texture_counts[3];

		current_mesh->textures = (struct Texture *)malloc((current_mesh->texture_count)* sizeof(struct Texture));
		Model_LoadMaterialTextures(model, current_mesh->textures, 0, texture_counts[0], material, aiTextureType_DIFFUSE, "texture_diffuse");
		Model_LoadMaterialTextures(model, current_mesh->textures, texture_counts[0], texture_counts[0] + texture_counts[1], material, aiTextureType_SPECULAR, "texture_specular");
		Model_LoadMaterialTextures(model, current_mesh->textures, texture_counts[0] + texture_counts[1], texture_counts[0] + texture_counts[1] + texture_counts[2], material, aiTextureType_HEIGHT, "texture_normal");
		Model_LoadMaterialTextures(model, current_mesh->textures, texture_counts[0] + texture_counts[1] + texture_counts[2], current_mesh->texture_count, material, aiTextureType_AMBIENT, "texture_height");
	}

	Mesh_Setup(current_mesh);
	model->mesh_count++;
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
			if (strcmp(model->textures_loaded[j].path.data, string.data) == 0)
			{
				textures[k++] = model->textures_loaded[j];
				skip = 1;
				break;
			}
		}

		if (!skip)
		{
			textures[k].tbo = Model_TextureFromFile(string.data, model->directory, 0);
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

	GLuint tbo;
	glGenTextures(1, &tbo);
	GLint width, height;
	unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, tbo);
	glTexImage2D(GL_TEXTURE_2D, 0, gamma ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return tbo;
}

void Model_Render(GLuint id, GLuint shader_program)
{
	GLuint i;
	for (i = 0; i < model_list[id].mesh_count; ++i)
	{
		Mesh_Render(&model_list[id].meshes[i], shader_program);
	}
}

void Model_Destroy()
{
	GLuint i;
	GLuint j;
	for (i = 0; i < num_models; ++i)
	{
		for (j = 0; j < model_list[i].mesh_count; ++j)
			Mesh_Destroy(&(model_list[i].meshes[j]));
		
		free(model_list[i].meshes);
		free(model_list[i].textures_loaded);
	}

	free(model_list);
}