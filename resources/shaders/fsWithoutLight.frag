#version 330 core

#define NB_MAX_LIGHTS 300

out vec4 color;
in vec2 f_uvs;
in vec3 f_normals;

in vec3 f_location;

struct Light
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  vec4 location;

  vec4 spotDirection;
  float light_cutOff;
  float light_outerCutOff;

  float constant, linear, quadratic;
  float brillance;
  int lightType;
};

// uniform block
uniform lightsBlock
{
  Light lights[NB_MAX_LIGHTS];
};

uniform int nbCurrentLights;

uniform mat4 camera;
uniform mat4 model;

uniform bool isColored = false;
uniform sampler2D f_texture;

uniform vec3 cameraLocation;

//========================= Basic Light Effects =========================\\ 

vec4 getDiffuse(vec4 diffuseValue, vec4 lightLocation)
{
  vec3 f_normal = normalize(f_normals);

  vec3 lightDir = normalize(lightLocation.xyz - f_location);
  
  float cosTeta = max(dot(lightDir, f_normal), 0);

  return diffuseValue * cosTeta;
}

vec4 getDiffuseDirectional(vec4 diffuseValue, vec4 lightLocation)
{
  vec3 f_normal = normalize(f_normals);

  vec3 lightDir = normalize(-lightLocation.xyz);
  vec3 transformedNormal = normalize(f_normal);
  
  float cosTeta = max(dot(lightDir, transformedNormal), 0);

  return diffuseValue * cosTeta;
}

vec4 getSpecular(vec4 specularValue, float brillance, vec4 lightLocation)
{
    vec3 lightDir = normalize(lightLocation.xyz - f_location);
    vec3 viewDir = normalize(cameraLocation - f_location);

    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normalize(f_normals)), 0.0);
    return specularValue * pow(specAngle, brillance);
}

//========================= Light Casters =========================\\ 

vec4 getPointLightIntensity(int i)
{
    float dist = length(lights[i].location.xyz - f_location);
    float attenuation = 1.0 / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * dist * dist);
    return attenuation * (lights[i].ambient + getDiffuse(lights[i].diffuse, lights[i].location) + getSpecular(lights[i].specular, lights[i].brillance, lights[i].location));
}

vec4 getDirectionalIntensity(int i)
{
  return lights[i].ambient 
        + getDiffuseDirectional(lights[i].diffuse, lights[i].location) 
        + getSpecular(lights[i].specular, lights[i].brillance, lights[i].location);
}

vec4 getFlashLightIntensity(int i)
{
  // setting up light
  vec3 lightDir = normalize(lights[i].location.xyz - f_location);

  float theta = dot(lightDir, normalize(-lights[i].spotDirection.xyz));
  if (theta > lights[i].light_cutOff)
  {
    return lights[i].ambient 
          + getDiffuse(lights[i].diffuse, lights[i].location) 
          + getSpecular(lights[i].specular, lights[i].brillance, lights[i].location);
  }
  else 
  {
    return lights[i].ambient;
  }
}

vec4 getSpotLightIntensity(int i)
{
  // setting up light
  vec3 lightDir = normalize(lights[i].location.xyz - f_location);

  float theta = dot(lightDir, normalize(-lights[i].spotDirection.xyz));
  float epsilon = (lights[i].light_cutOff - lights[i].light_outerCutOff);
  float spotIntensity = clamp((theta - lights[i].light_outerCutOff) / epsilon, 0.0, 1.0);

  return lights[i].ambient 
        + (getDiffuse(lights[i].diffuse, lights[i].location) 
        + getSpecular(lights[i].specular, lights[i].brillance, lights[i].location)) * spotIntensity;
}


//========================= Main =========================\\ 

void main()
{
    vec4 intensity = vec4(1.0,1.0,1.0,1);

    color = texture(f_texture, f_uvs) * intensity;
}
