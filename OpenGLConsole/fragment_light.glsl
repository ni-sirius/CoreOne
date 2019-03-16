#version 450

struct Light
{
  vec3 lightPos;
  vec3 lightColor;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Light light0;
uniform vec3 lightColor;

void main()
{
  fs_color = vec4(light0.lightColor, 1.f);
}