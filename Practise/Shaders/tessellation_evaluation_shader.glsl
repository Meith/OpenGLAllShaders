#version 440 core

layout (triangles, equal_spacing, cw) in;

in vec2 tcs_tex_coords[];

out vec2 tes_tex_coords;

void main()
{ 
	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + (gl_TessCoord.y * gl_in[1].gl_Position) + (gl_TessCoord.z * gl_in[2].gl_Position);
	tes_tex_coords = (gl_TessCoord.x * tcs_tex_coords[0]) + (gl_TessCoord.y * tcs_tex_coords[1]) + (gl_TessCoord.z * tcs_tex_coords[2]);
}          