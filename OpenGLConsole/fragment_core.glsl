#version 450

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffudeTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform vec3 LightPos0;
uniform vec3 CameraPos;

vec3 calculateAmbient(Material material)
{
  return material.ambient;
}
vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 LightPos0)
{
  vec3 posToLightDirVec = normalize(LightPos0 - vs_position);
  //vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
  float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
  return  material.diffuse * diffuse;
}
vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 LightPos0, vec3 CameraPos)
{
  vec3 lightToPosDirVec = normalize(vs_position - LightPos0);
  vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
  vec3 posToViewDirVec = normalize(CameraPos - vs_position);
  float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 32);
  return material.specular * specularConstant;
}

void main()
{
  //fs_color = vec4(vs_color, 1.f);
  vec3 ambientFinal = calculateAmbient(material);
  vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, LightPos0);
  vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, LightPos0, CameraPos);

  //attenuation

  //final
  fs_color = texture(material.diffudeTex, vs_texcoord) * //texture(material.specularTex, vs_texcoord) *
             (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}