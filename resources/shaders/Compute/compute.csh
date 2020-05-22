#version 320 es
precision highp float;
precision highp int;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0, std430) readonly buffer InputData {
  vec4 inputData[];
};

layout(binding = 1, std430) writeonly buffer OutputData {
  vec4 outputData[];
};

void main(void) {
  outputData[ gl_LocalInvocationID.x ] = inputData[gl_LocalInvocationID.x] + vec4(1);
}