#version 330 core

out vec4 fragColor;

struct Material {
	sampler2D diffuse;
	vec3      specular;
	float     shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3     xyz;
uniform Material xyzMat;
uniform Light    xyzLht;

void main() {
	vec3 ambient = xyzLht.ambient * texture(xyzMat.diffuse, texCoords).rgb;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(xyzLht.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = xyzLht.diffuse * diff * texture(xyzMat.diffuse, texCoords).rgb;

	// specular
	vec3 viewDir = normalize(xyz - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), xyzMat.shininess);
	vec3 specular = xyzLht.specular * (spec * xyzMat.specular);

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}
