#version 300 es
precision highp float;
precision highp int;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 PVM;

out vec2 TexCoord;

void main() {
	gl_Position = PVM * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}