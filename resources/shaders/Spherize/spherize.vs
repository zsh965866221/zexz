#version 300 es
precision highp float;
precision highp int;

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec2 aTexCoord1;

uniform mat4 uMVPMatrix;
uniform mat4 uTexuvMat1;
uniform vec2 uTextureSize1;

out vec2 vTexCoord1;

void main() {
	gl_Position = uMVPMatrix * aPosition;
	vTexCoord1 = (uTexuvMat1 * vec4(aTexCoord1, 0.0, 1.0)).xy;
}