#version 300 es
precision highp float;
precision highp int;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;

void main() {
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}