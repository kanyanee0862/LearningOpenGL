#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};
struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight
{
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutoff;
	float outercutoff;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 lightColor;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

#define NR_POINT_LIGHT 4
uniform PointLight pointLights[NR_POINT_LIGHT];

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

//function
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

void main()
{
	//properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	//Directional lighting
	vec3 result = CalcDirLight(dirLight,norm,viewDir);
	//Point lights
	for(int i = 0; i< NR_POINT_LIGHT; ++i)
	{
		result += CalcPointLight(pointLights[i],norm,FragPos,viewDir);
	}
	//Spot Light
	result += CalcSpotLight(spotLight,norm,FragPos,viewDir);
	FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse shading
	float diff = max(dot(normal,lightDir),0.0);
	//specular shading
	vec3 reflecDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
	//combine result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords).rgb);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords).rgb);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords).rgb);
	return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	//diffuse shading 
	float diff = max(dot(normal,lightDir),0.0);
	//specular shading
	vec3 reflecDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
	
	//Calculate attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	//combine result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords).rgb);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords).rgb);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords).rgb);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	//diffuse shading 
	float diff = max(dot(normal,lightDir),0.0);
	//specular shading
	vec3 reflecDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
	
	//spotlight
	float theta = dot(lightDir,normalize(-light.direction));
	float epsilon = (light.cutoff - light.outercutoff);
	float intensity = clamp((theta - light.outercutoff)/epsilon,0.0,1.0);
	
	//Calculate attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	//combine result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords).rgb);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords).rgb);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords).rgb);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}