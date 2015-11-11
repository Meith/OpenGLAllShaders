#include "Game.h"
#include "InputHandler.h"
#include "Shaders.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"

#define MAX_SHADERS 1
#define MAX_TRANSFORMS 1
#define MAX_CAMERAS 1
#define MAX_MODELS 1
#define MAX_GAME_OBJECTS 1

static struct GameObject *game_object_list = NULL;
static struct CameraObject *camera_object_list = NULL;

static GLuint num_objects;
static GLuint num_cams;

void Game_Init()
{
	InputHandler_Init();

	Shaders_Init(MAX_SHADERS);
	Model_Init(MAX_MODELS);
	Transform_Init(MAX_TRANSFORMS);
	Camera_Init(MAX_CAMERAS);

	game_object_list = (struct GameObject *)malloc(MAX_GAME_OBJECTS * sizeof(struct GameObject));
	camera_object_list = (struct CameraObject *)malloc(MAX_CAMERAS * sizeof(struct CameraObject));
	num_objects = 0;
	num_cams = 0;

	struct ShaderPair simple_shader_pair[2] = { [0].shader_source = "Shaders/vertex_shader.glsl", [0].shader_type = GL_VERTEX_SHADER, [1].shader_source = "Shaders/fragment_shader.glsl", [1].shader_type = GL_FRAGMENT_SHADER };
	GLuint simple_shader_id = Shaders_CreateShaderProgram(&simple_shader_pair[0], 2);

	GLuint simple_model_id = Model_Add("Objects/Nanosuit/nanosuit.obj");

	vec3 pos = { 0.0f, 0.0f, 0.0f };
	vec3 rot = { 0.0f, 0.0f, 0.0f };
	vec3 scale = { 1.0f, 1.0f, 1.0f };
	GLuint simple_transform_id = Transform_Add(pos, rot, scale);

	game_object_list[num_objects].shader_id = simple_shader_id;
	game_object_list[num_objects].model_id = simple_model_id;
	game_object_list[num_objects].transform_id = simple_transform_id;
	num_objects++;

	vec3 eye = { 0.0f, 0.0f, 10.0f };
	vec3 look = { 0.0f, 0.0f, -1.0f };
	vec3 up = { 0.0f, 1.0f, 0.0f };
	vec4 fanf = { 0.785398f, 1.333333f, 0.1f, 100.0f };
	GLuint simple_camera_id = Camera_Add(eye, look, up, fanf);
	camera_object_list[num_cams].camera_id = simple_camera_id;
	num_cams++;
}

GLuint Game_HandleInput()
{
	return InputHandler_Update();
}

void Game_FixedUpdate(GLfloat t, GLfloat const dt)
{

}

void Game_Update()
{
	Transform_Update();
}

void Game_LateUpdate()
{
	Camera_Update();
}

void Game_Render()
{
	Shaders_UseProgram(game_object_list[0].shader_id);
	{
		Camera_Render(camera_object_list[0].camera_id);
		Transform_Render(game_object_list[0].transform_id);
		Model_Render(game_object_list[0].model_id, Shaders_GetProgram(game_object_list[0].shader_id));
	}
	Shaders_UnuseProgram();
}

void Game_Destroy()
{
	InputHandler_Destroy();

	Shaders_Destroy();
	Transform_Destroy();
	Camera_Destroy();

	free(game_object_list);
	free(camera_object_list);
}