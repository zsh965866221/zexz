#version 300 es
precision highp float;
precision highp int;

in vec2 vTexCoord1;
in vec3 vNorm;
in vec3 vFragPos;
in float vS;

uniform sampler2D uBitmap1;

uniform bool uSpecular;

const vec3 viewPos = vec3(0.0, 0.0, -3.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float shininess = 32.0;

void main() {
	vec3 lightPos = vec3(1.2 * vS, 1.2 * vS, -1.0);
	vec3 lightDir = normalize(lightPos - vFragPos);
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, normalize(vNorm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	gl_FragColor = texture2D(uBitmap1, vTexCoord1);
	if (uSpecular == true) {
		gl_FragColor += vec4(spec * lightColor, 1.0);
	}
}