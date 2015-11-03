#ifndef __Types__
#define __Types__

#include "Vector.h"

#include <SDL/SDL.h>
#include <assimp/types.h>

struct Window
{
	SDL_Window *gl_window;
	SDL_GLContext gl_context;
	GLuint width;
	GLuint height;
	GLenum flags;
};

struct Input
{
	GLuint mouse_button_state[3];
	vec2 mouse_pos;
	Uint8 const *keyboard_state;
	SDL_Event sdl_event;
};

struct ShaderPair
{
	GLchar const *shader_source;
	GLenum shader_type;
};

struct Transform
{
	vec3 pos;
	vec4 rot;
	vec3 scale;
	mat4x4 model_matrix;
};

struct Camera
{
	vec3 eye;
	vec3 look;
	vec3 up;
	mat4x4 view_matrix;
	mat4x4 perspective_matrix;
};

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texcoords;
	vec3 tangent;
	vec3 bitangent;
};

struct Texture
{
	GLuint tbo;
	GLchar type[25];
	struct aiString path;
};

struct Mesh
{
	struct Texture *textures;
	GLuint texture_count;

	struct Vertex *vertices;
	GLuint vertex_count;

	GLuint *indices;
	GLuint index_count;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

struct Model
{
	struct Texture *textures_loaded;
	GLuint textures_loaded_count;
	GLchar directory[50];

	struct Mesh *meshes;
	GLuint mesh_count;
};

struct GameObject
{
	GLuint shader_id;
	GLuint model_id;
	GLuint transform_id;
};

#endif