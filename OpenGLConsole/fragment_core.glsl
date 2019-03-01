#version 450

struct Light
{
  vec3 lightPos;
  vec3 lightColor;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
  float shiness;

	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Light light0;
uniform Material material;
uniform vec3 CameraPos;

vec3 calculateAmbient(Material material)
{
  return material.ambient * light0.lightColor;
}
vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos)
{
  vs_normal = normalize(vs_normal);
  vec3 posToLightDirVec = normalize(lightPos - vs_position);
  float diffuse = max(dot(vs_normal, posToLightDirVec), 0.f);
  return  light0.lightColor * diffuse;
}
vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos, vec3 CameraPos)
{
  vec3 lightToPosDirVec = normalize(vs_position - lightPos);
  vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
  vec3 posToViewDirVec = normalize(CameraPos - vs_position);
  float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), material.shiness);
  return material.specular * specularConstant * texture(material.specularTex, vs_texcoord).rgb;

}

void main()
{
  vec3 ambientFinal = calculateAmbient(material);
  vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, light0.lightPos);
  vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, light0.lightPos, CameraPos);

  //attenuation

  //final
  fs_color = texture(material.diffuseTex, vs_texcoord) *
             (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
  //fs_color = vec4(vs_color, 1.f) *
  //           (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}