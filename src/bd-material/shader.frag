#version 330 core

in vec3 Normal;
in vec3 fragColor;
in vec3 fragPos;

out vec4 color;

struct Material {
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3     xyz;            // cam position
uniform Material xyzMat;
uniform Light    xyzLht;

void main() {
	// ambient
	vec3 ambient   = xyzLht.ambient * xyzMat.ambient;

	// diffuse
	vec3  norm     = normalize(Normal);
	vec3  lightDir = normalize(xyzLht.position - fragPos);
	float diff     = max(dot(norm, lightDir), 0.0);
	vec3  diffuse  = xyzLht.diffuse * diff * xyzMat.diffuse;

	// specular
	vec3  viewDir    = normalize(xyz - fragPos);
	vec3  reflectDir = reflect(-lightDir, norm);
	float spec       = pow(max(dot(viewDir, reflectDir), 0.0), xyzMat.shininess);
	vec3  specular   = xyzLht.specular * spec * xyzMat.specular;

	//vec3 result = ambient + diffuse + specular;
	vec3 result = (ambient + diffuse + specular) * fragColor;
	color       = vec4(result, 1.0);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

