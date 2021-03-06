#version 330 core
struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
void main()
{
	//ambient
	vec3 ambient = light.ambient * texture(material.ambient,TexCoords).rgb;
	
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * texture(material.diffuse,TexCoords).rgb;
	
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflecDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result,1.0);
}