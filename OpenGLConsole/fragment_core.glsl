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

#define NUM_POINT_LIGHTS 2
uniform PointLight pointLight[NUM_POINT_LIGHTS];
#define NUM_DIRECT_LIGHTS 1
uniform DirectLight dirLight[NUM_DIRECT_LIGHTS];
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

vec3 calculateDiffuseDirect(vec3 vs_normal, vec3 lightDirection, vec3 lightDiffuseColor)
{
  vs_normal = normalize(vs_normal);
  float diffuse = max(dot(vs_normal, normalize(lightDirection)), 0.f);
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
  vec3 result;

  for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
  {
    float attenuation = calculateAttenuation(vs_position, pointLight[i].position, pointLight[i].constant, pointLight[i].linear, pointLight[i].quadratic);

    vec3 ambientFinal = attenuation * calculateAmbient(pointLight[i].ambientColor);
    vec3 diffuseFinal = attenuation * calculateDiffuse(vs_position, vs_normal, pointLight[i].position, pointLight[i].diffuseColor);
    vec3 specularFinal = attenuation * calculateSpecular(material, vs_position, vs_normal, pointLight[i].position, pointLight[i].specularColor, CameraPos);

    result += ambientFinal + diffuseFinal + specularFinal;
  }

  return vec4(result, 1.f);
}

vec4 getDirectLight()
{
  vec4 result;

  for (int i = 0; i < NUM_DIRECT_LIGHTS; ++i)
  {
    vec3 lightDirection = normalize(-dirLight[i].direction);

    vec4 ambientFinal = vec4(calculateAmbient(dirLight[i].ambientColor), 1.f);
    vec4 diffuseFinal = vec4(calculateDiffuseDirect(vs_normal, lightDirection, dirLight[i].diffuseColor), 1.f);
    vec4 specularFinal = vec4(calculateSpecular(material, vs_position, vs_normal, lightDirection, dirLight[i].specularColor, CameraPos), 1.f);

    result += ambientFinal + diffuseFinal + specularFinal;
  }

  return result;
}

vec4 getSpotLights()
{
  vec3 result;

  for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
  {
    float attenuation = calculateAttenuation(vs_position, spotLight[i].position, spotLight[i].constant, spotLight[i].linear, spotLight[i].quadratic);
    float intensity = calculateConeIntensity(vs_position, spotLight[i].position, spotLight[i].direction, spotLight[i].cutoffAngle, spotLight[i].cutoffOuterAngle);

    vec3 ambientFinal = attenuation * spotLight[i].ambientColor;
    vec3 diffuseFinal = attenuation * intensity * calculateDiffuse(vs_position, vs_normal, spotLight[i].position, spotLight[i].diffuseColor);
    vec3 specularFinal = attenuation * intensity * calculateSpecular(material, vs_position, vs_normal, spotLight[i].position, spotLight[i].specularColor, CameraPos);

    result += ambientFinal + diffuseFinal + specularFinal;
  }

  return vec4(result, 1.f);
}


float LinearizeDepth(float depth)
{
  float zNear = 0.1f;
  float zFar  = 100.0f;

  // преобразуем обратно в NDC
  float z = depth * 2.0 - 1.0;
  return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
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


  //Depth visualization
  //float depth = LinearizeDepth(gl_FragCoord.z) / zFar;
  //fs_color = vec4(vec3(depth), 1.0);
}