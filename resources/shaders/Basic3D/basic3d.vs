#version 300 es
precision highp float;
precision highp int;

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec2 aTexCoord1;

uniform mat4 uMVPMatrix;
uniform mat4 uTexuvMat1;
uniform vec2 uTextureSize1;

uniform float uSwivel;
uniform float uTilt;
uniform float uDistance;

out vec2 vTexCoord1;
out vec3 vNorm;
out vec3 vFragPos;
out float vS;

const float PI = 3.1415926535898;
const float scaleFactor = 500.0;

// https://en.wikipedia.org/wiki/Rotation_matrix
mat4 rotateMatrix(vec3 axis, float theta) {
  axis = normalize(axis);
  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  float ct = cos(theta);
  float st = sin(theta);
  float ct1 = 1.0 - ct;
  return mat4(
    ct + x * x * ct1,       x * y * ct1 - z * st,     x * y * ct1 + y * st, 		0.0,
    y * x * ct1 + z * st,   ct + y * y * ct1,         y * z * ct1 - x * st, 		0.0,
    z * x * ct1 - y * st,   z * y * ct1 + x * st,     ct + z * z * ct1,     		0.0,
		0.0,										0.0, 											0.0,											1.0
  );
}

mat4 translateMatrix(vec3 t) {
	return mat4(
		1.0,		0.0,		0.0,		t.x,
		0.0,		1.0, 		0.0, 		t.y,
		0.0, 		0.0, 		1.0, 		t.z,
		0.0, 		0.0, 		0.0, 		1.0
	);
}

mat4 scaleMatrix(vec3 s) {
	return mat4(
		s.x,		0.0, 		0.0, 		0.0,
		0.0,		s.y,		0.0, 		0.0,
		0.0, 		0.0,		s.z,		0.0,
		0.0,		0.0,		0.0,		1.0
	);
}

void main() {
	mat4 rotateX = rotateMatrix(vec3(1.0, 0.0, 0.0), radians(uTilt));
	mat4 rotateY = rotateMatrix(vec3(0.0, 1.0, 0.0), radians(uSwivel));
	float s = 1.0 - uDistance * 1.0 / scaleFactor;
	s = clamp(s, 0.0, 5.0);
	vS = s;
	mat4 scaleS = scaleMatrix(vec3(s, s, 0.0));

	gl_Position = rotateX * rotateY * scaleS * aPosition;
	vFragPos = gl_Position.xyz;
	
	gl_Position = uMVPMatrix * gl_Position;
	vTexCoord1 = (uTexuvMat1 * vec4(aTexCoord1, 0.0, 1.0)).xy;

	vNorm = (rotateX * rotateY * vec4(0.0, 0.0, -1.0, 0.0)).xyz;
}