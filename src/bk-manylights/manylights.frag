#version 330 core

out vec4 fragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float     shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3  position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3  position;
	vec3  direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 4

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3       xyz;
uniform Material   xyzmat;
uniform DirLight   xyzdl;
uniform PointLight xyzpl[NR_POINT_LIGHTS];
uniform SpotLight  xyzsl;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3  lightDir = normalize(-light.direction);
	float diff     = max(dot(normal, lightDir), 0.0);

	vec3  reflectDir = reflect(-lightDir, normal);
	float spec       = pow(max(dot(viewDir, reflectDir), 0.0), xyzmat.shininess);

	vec3 ambient  = light.ambient * vec3(texture(xyzmat.diffuse, texCoords));
	vec3 diffuse  = light.diffuse * diff * vec3(texture(xyzmat.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(xyzmat.specular, texCoords));

	return ambient * diffuse * specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3  lightDir = normalize(light.position - fragPos);
	float diff     = max(dot(normal, lightDir), 0.0);

	vec3  reflectDir = reflect(-lightDir, normal);
	float spec       = pow(max(dot(viewDir, reflectDir), 0.0), xyzmat.shininess);

	float distance = length(light.position - fragPos);
	float atten    = 1.0 / (light.constant + light.linear
	                        * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(xyzmat.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(xyzmat.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(xyzmat.specular, texCoords));

	ambient  *= atten;
	diffuse  *= atten;
	specular *= atten;

	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
	vec3  lightDir = normalize(light.position - fragPos);
	float diff     = max(dot(normal, lightDir), 0.0);

	vec3  reflectDir  = reflect(-lightDir, normal);
	float spec        = pow(max(dot(viewDir, reflectDir), 0.0), xyzmat.shininess);
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear
	                           * distance + light.quadratic * (distance * distance));    

	float theta     = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient  = light.ambient * vec3(texture(xyzmat.diffuse, texCoords));
	vec3 diffuse  = light.diffuse * diff * vec3(texture(xyzmat.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(xyzmat.specular, texCoords));

	ambient  *= attenuation * intensity;
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}

void main() {
	vec3 norm    = normalize(normal);
	vec3 viewDir = normalize(xyz - fragPos);

	vec3 result = calcDirLight(xyzdl, norm, viewDir);

	for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
		result += calcPointLight(xyzpl[i], norm, viewDir);
	}

	result += calcSpotLight(xyzsl, norm, viewDir);

	fragColor = vec4(result, 1.0);
}
