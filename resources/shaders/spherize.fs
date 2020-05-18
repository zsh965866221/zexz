#version 300 es
precision highp float;
precision highp int;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform float Radius;
uniform vec2 Center;
uniform vec2 TexutreSize;

// texture sampler
uniform sampler2D texture1;

const float epsilon = 1e-5;

/** \brief the function based r, please visit: https://www.desmos.com/calculator/py5yksrcna */
void main() {
	float aspect = TexutreSize.y / TexutreSize.x;

	vec2 xy = TexCoord * 2.0 - 1.0;
	vec2 uv = xy - Center;
	uv *= vec2(1.0, aspect);

	float r = length(uv);
	float theta = atan(uv.y, uv.x);
	
	if (r < Radius + epsilon) {
		float k = r / Radius;
		float f = 
			+ 0.20 * pow(k, 4.0)
			+ 0.20 * pow(k, 3.0)
			+ 0.00 * pow(k, 2.0)
			+ 0.60 * pow(k, 1.0);
		r = f * Radius;
	}

	uv.x = r * cos(theta);
	uv.y = r * sin(theta);

	xy = uv / vec2(1.0, aspect);
	xy = xy + Center;
	xy = (xy + 1.0) / 2.0;
	
	FragColor = texture(texture1, xy);
}