#version 450

struct PointLight
{
  vec3 position;

  vec3 ambientColor;
  vec3 diffuseColor; // Main light color
  vec3 specularColor;

  float constant;
  float linear;
  float quadratic;
};

struct DirectLight
{
  vec3 direction; // postion from light

  vec3 ambientColor;
  vec3 diffuseColor; // Main light color
  vec3 specularColor;
};

struct SpotLight
{
  vec3 position;
  vec3 direction;

  float cutoffAngle;
  float cutoffOuterAngle;

  vec3 ambientColor;
  vec3 diffuseColor; // Main light color
  vec3 specularColor;

  float constant;
  float linear;
  float quadratic;
};

struct Material
{
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

#define NUM_POINT_LIGHTS 1
uniform PointLight pointLight[NUM_POINT_LIGHTS];
uniform DirectLight dirLight0;
#define NUM_SPOT_LIGHTS 1
uniform SpotLight spotLight[NUM_SPOT_LIGHTS];

uniform Material material;
uniform vec3 CameraPos;

//Helpers
vec3 calculateAmbient(vec3 lightAmbientColor)
{
  return lightAmbientColor;
}

vec3 calculateDiffuse(vec3 vs_position, vec3 vs_normal, vec3 lightPos, vec3 lightDiffuseColor)
{
  vs_normal = normalize(vs_normal);
  vec3 posToLightDirVec = normalize(lightPos - vs_position);
  float diffuse = max(dot(vs_normal, posToLightDirVec), 0.f);
  return  lightDiffuseColor * diffuse;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos, vec3 lightSpecularColor, vec3 CameraPos)
{
  vec3 lightToPosDirVec = normalize(vs_position - lightPos);
  vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
  vec3 posToViewDirVec = normalize(CameraPos - vs_position);
  float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), material.shiness);
  return lightSpecularColor * specularConstant * texture(material.specularTex, vs_texcoord).rgb;
}

float calculateAttenuation(vec3 vs_position, vec3 lightPos, float constant, float linear, float quadratic)
{
  float dist = length(lightPos - vs_position);

  return 1.f / (constant +
                linear * dist +
                quadratic * (dist* dist));
}

float calculateConeIntensity(vec3 vs_position ,vec3 lightPos, vec3 lightDir, float cutoff, float outerCutoff)
{
  vec3 posToLightDirVec = normalize(lightPos - vs_position);
  float theta = dot(posToLightDirVec, normalize(-lightDir));

  float epsilon = cutoff - outerCutoff;
  return clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);
}

//Lights Type Calc
vec4 getPointLights()
{
  vec4 result;

  for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
  {
    float attenuation = calculateAttenuation(vs_position, pointLight[i].position, pointLight[i].constant, pointLight[i].linear, pointLight[i].quadratic);

    vec4 ambientFinal = attenuation * vec4(calculateAmbient(pointLight[i].ambientColor), 1.f);
    vec4 diffuseFinal = attenuation * vec4(calculateDiffuse(vs_position, vs_normal, pointLight[i].position, pointLight[i].diffuseColor), 1.f);
    vec4 specularFinal = attenuation * vec4(calculateSpecular(material, vs_position, vs_normal, pointLight[i].position, pointLight[i].specularColor, CameraPos), 1.f);

    result = ambientFinal + diffuseFinal + specularFinal;
  }

  return result;
}

vec4 getDirectLight()
{
  vec3 lightDirection = normalize(-dirLight0.direction);

  vec4 ambientFinal = vec4(calculateAmbient(dirLight0.ambientColor), 1.f);
  vec4 diffuseFinal = vec4(calculateDiffuse(vs_position, vs_normal, lightDirection, dirLight0.diffuseColor), 1.f);
  vec4 specularFinal = vec4(calculateSpecular(material, vs_position, vs_normal, lightDirection, dirLight0.specularColor, CameraPos), 1.f);

  return ambientFinal + diffuseFinal + specularFinal;
}

vec4 getSpotLights()
{
  vec4 result;

  for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
  {
    float attenuation = calculateAttenuation(vs_position, spotLight[i].position, spotLight[i].constant, spotLight[i].linear, spotLight[i].quadratic);
    float intensity = calculateConeIntensity(vs_position, spotLight[i].position, spotLight[i].direction, spotLight[i].cutoffAngle, spotLight[i].cutoffOuterAngle);

    vec4 ambientFinal = attenuation * vec4(spotLight[i].ambientColor, 1.f);
    vec4 diffuseFinal = attenuation * intensity * vec4(calculateDiffuse(vs_position, vs_normal, spotLight[i].position, spotLight[i].diffuseColor), 1.f);
    vec4 specularFinal = attenuation * intensity * vec4(calculateSpecular(material, vs_position, vs_normal, spotLight[i].position, spotLight[i].specularColor, CameraPos), 1.f);

    result = ambientFinal + diffuseFinal + specularFinal;
  }

  return result;
}


void main()
{
  vec4 lighting = getPointLights() + getDirectLight() + getSpotLights();

  //final
  vec4 fragFinalColor;
  if (bool(material.useColors))
    fragFinalColor= vec4(vs_color, 1.f);
  else
    fragFinalColor = texture(material.diffuseTex, vs_texcoord);

  fs_color = fragFinalColor * lighting;
}