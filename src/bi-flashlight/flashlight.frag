#version 330 core

out vec4 fragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float     shininess;
};

struct Light {
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

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3     xyz;
uniform Material xyzMat;
uniform Light    xyzLht;

void main() {
	vec3  lightDir = normalize(xyzLht.position - fragPos);

	float theta = dot(lightDir, normalize(-xyzLht.direction));

	if (theta > xyzLht.cutOff) {
		vec3 ambient = xyzLht.ambient * texture(xyzMat.diffuse, texCoords).rgb;

		// diffuse
		vec3  norm     = normalize(normal);
		float diff     = max(dot(norm, lightDir), 0.0);
		vec3  diffuse  = xyzLht.diffuse * diff
										 * texture(xyzMat.diffuse, texCoords).rgb;

		// specular
		vec3  viewDir    = normalize(xyz - fragPos);
		vec3  reflectDir = reflect(-lightDir, norm);
		float spec       = pow(max(dot(viewDir,reflectDir), 0.0), xyzMat.shininess);
		vec3  specular   = xyzLht.specular * spec
		                   * texture(xyzMat.specular, texCoords).rgb;

		float distance    = length(xyzLht.position - fragPos);
		float attenuation = 1.0 / (xyzLht.constant + xyzLht.linear * distance
		                           + xyzLht.quadratic * (distance * distance));

		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular;
		fragColor = vec4(result, 1.0);
	} else {
		fragColor = vec4(xyzLht.ambient*texture(xyzMat.diffuse,texCoords).rgb, 1.0);
	}
}
