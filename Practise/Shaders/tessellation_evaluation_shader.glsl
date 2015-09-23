#version 440 core

layout (triangles, equal_spacing, cw) in;

in vec3 tcs_normal[];
in vec2 tcs_texcoords[];
in vec3 tcs_tangent[];
in vec3 tcs_bitangent[];


out vec3 tes_normal;
out vec2 tes_texcoords;
out vec3 tes_tangent;
out vec3 tes_bitangent;

void main()
{ 
	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + (gl_TessCoord.y * gl_in[1].gl_Position) + (gl_TessCoord.z * gl_in[2].gl_Position);
	tes_normal = (gl_TessCoord.x * tcs_normal[0]) + (gl_TessCoord.y * tcs_normal[1]) + (gl_TessCoord.z * tcs_normal[2]);
	tes_texcoords = (gl_TessCoord.x * tcs_texcoords[0]) + (gl_TessCoord.y * tcs_texcoords[1]) + (gl_TessCoord.z * tcs_texcoords[2]);
	tes_tangent = (gl_TessCoord.x * tcs_tangent[0]) + (gl_TessCoord.y * tcs_tangent[1]) + (gl_TessCoord.z * tcs_tangent[2]);
	tes_bitangent = (gl_TessCoord.x * tcs_bitangent[0]) + (gl_TessCoord.y * tcs_bitangent[1]) + (gl_TessCoord.z * tcs_bitangent[2]);
}          