#version 450

struct Light
{
  vec3 lightPos;

  vec3 lightAmbientCol;
  vec3 lightDiffuseCol; // Main light color
  vec3 lightSpecularCol;
};

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

uniform Light light0;
uniform Material material;
uniform vec3 CameraPos;


vec3 calculateAmbient(Material material)
{
  return material.ambient * light0.lightAmbientCol;
}
vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos)
{
  vs_normal = normalize(vs_normal);
  vec3 posToLightDirVec = normalize(lightPos - vs_position);
  float diffuse = max(dot(vs_normal, posToLightDirVec), 0.f);
  return  light0.lightDiffuseCol * diffuse;
}
vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos, vec3 CameraPos)
{
  vec3 lightToPosDirVec = normalize(vs_position - lightPos);
  vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
  vec3 posToViewDirVec = normalize(CameraPos - vs_position);
  float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), material.shiness);
  return light0.lightSpecularCol * specularConstant * texture(material.specularTex, vs_texcoord).rgb;
}

void main()
{
  vec4 ambientFinal = vec4(calculateAmbient(material), 1.f);// * texture(material.diffuseTex, vs_texcoord);
  vec4 diffuseFinal = vec4(calculateDiffuse(material, vs_position, vs_normal, light0.lightPos), 1.f);// * texture(material.diffuseTex, vs_texcoord);
  vec4 specularFinal = vec4(calculateSpecular(material, vs_position, vs_normal, light0.lightPos, CameraPos), 1.f);

  //attenuation

  //final
  vec4 fragFinalColor;
  if (bool(material.useColors))
    fragFinalColor= vec4(vs_color, 1.f);
  else
    fragFinalColor = texture(material.diffuseTex, vs_texcoord);
  

  fs_color = fragFinalColor * (ambientFinal + diffuseFinal + specularFinal);
}