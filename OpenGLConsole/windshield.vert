#version 450

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ProjectionMat;

void main()
{
	//vs_position =  vec3(vertex_position.xy, 0.1f);
	vs_color = vertex_color;
	//vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	//vs_normal = vec3(0.f, 0.f, 1.f);

	gl_Position = ProjectionMat * vec4(vertex_position.xy, -0.9f, 1.f);
}