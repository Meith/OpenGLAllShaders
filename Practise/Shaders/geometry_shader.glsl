#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 view_mat;

uniform int exp;

void main()
{
	vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = normalize(cross(ab, ac));
	vec3 trans_normal = (view_mat * vec4(normal, 0.0)).xyz;

	int i;
	for(i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position + vec4(exp * trans_normal, 0.0);
		EmitVertex();
	}

	EndPrimitive();
}