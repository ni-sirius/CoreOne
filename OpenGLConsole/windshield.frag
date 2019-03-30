#version 450

struct Material
{
	vec3 ambient;
  float shiness;

  int useColors;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

void main()
{
  vec4 fragFinalColor;
  if (bool(material.useColors))
    fragFinalColor= vec4(vs_color, 1.f);
  else
    fragFinalColor = texture(material.diffuseTex, vs_texcoord);

  fs_color = fragFinalColor;
}