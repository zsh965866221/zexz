#version 300 es
precision highp float;
precision highp int;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

// texture sampler
uniform sampler2D texture1;

// light
uniform vec3 norm;
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform bool Specular;

// const vec3 lightPos = vec3(1.0, 1.0, -1.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float shininess = 64.0;

void main() {
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalize(norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	FragColor = texture(texture1, TexCoord);
	if (Specular == true) {
		FragColor += vec4(spec * lightColor, 1.0);
	}
}